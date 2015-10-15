
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>

#define MAXLINE 1023
#define LISTENQ 10
#define PORT    33000
#define MAXN    16384
#define MAX_PROCESS_NUM  16

typedef struct 
{
	pid_t child_pid;
	int child_pipefd;
	int child_status;
	long child_count;
	/* data */
}Child;

Child *cptr;

void web_child(int sockfd)
{
	int ntowrite;
	ssize_t nread;
	char line[MAXLINE], result[MAXLINE];

	for(;;)
	{
		if((nread = read(sockfd, line, MAXLINE)) == 0)
			return;

		ntowrite = atol(line);
		if((ntowrite <= 0) || (ntowrite > MAXN))
		{
			printf("client requst for %d bytes\n", ntowrite);
			return;
		} 
		write(sockfd, result, ntowrite);
	}
}

void child_main(int i, int listendfd, int addrlen)
{
	char c;
	int connfd;
	ssize_t n;

	printf("child %ld starting\n", (long)getpid());
	for(;;)
	{
		if((n = recv(STDERR_FILENO, &c, i, &connfd)) == 0)
		{
			printf("read return\n");
			return;
		}

		if(connfd < 0)
		{
			printf("no desc from read\n");
		}

		web_child(connfd);
		close(connfd);
		write(STDERR_FILENO, "", 1);
	}
}

pid_t child_make(int i, int listendfd, int addrlen)
{
	int sockfd[2];
	pid_t pid;
	
	if((pid = fork()) > 0)
	{
		close(sockfd[1]);
		cptr[i].child_pid = pid;
		cptr[i].child_pipefd = sockfd[0];
		cptr[i].child_status = 0;
		return pid;
	}

	dup2(sockfd[1], STDERR_FILENO);
	close(sockfd[0]);
	close(sockfd[1]);
	close(listendfd);
	child_main(i, listendfd, addrlen);
}

int Socket(int family, int type, int protocol)
{
	int n = 0;
	if((n = socket(family,type,protocol)) < 0)
		perror("socket error");

	return n;
}


void sig_chld(int signal)
{
	pid_t pid;
	int stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("child %d terminated\n", pid);
	}

	return;
}

void sig_int(int signal)
{
	return;
}

int main(int argc, char const *argv[])
{
	int i = 0;
	int nsel = 0;
	int navail = 0;
	int listendfd, connfd;
	int maxfd;

	fd_set rset, masterset;
	ssize_t n;

	socklen_t addrlen, clilen;
	struct sockaddr_in servaddr, cliaddr;

	char buff[MAXLINE + 1] = {0};

	listendfd = Socket(AF_INET, SOCK_STREAM,0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listendfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listendfd, LISTENQ);

	FD_ZERO(&masterset);
	FD_SET(listendfd, &masterset);

	maxfd = listendfd;

	navail = MAX_PROCESS_NUM;

	//初始化进程池
	cptr = malloc(sizeof(Child) * MAX_PROCESS_NUM);
	if(cptr == NULL)
	{
		printf("malloc failure");
		return -1;
	}

	for(i = 0; i < MAX_PROCESS_NUM; ++i)
	{
		child_make(i,listendfd,addrlen);
		FD_SET(cptr[i].child_pipefd, &masterset);
		if(maxfd < cptr[i].child_pipefd)
		{
			maxfd = cptr[i].child_pipefd;
		}
	}

	signal(SIGCHLD, sig_chld);
	signal(SIGINT, sig_int);

	maxfd = listendfd;
	for(;;)
	{
		rset = masterset;
		if(navail <= 0)
		{
			FD_CLR(listendfd, &rset);
		}

		nsel = select(maxfd + 1, &rset, NULL,NULL,NULL);
		if(FD_ISSET(listendfd, &rset))
		{
			clilen = sizeof(cliaddr);
			connfd = accept(listendfd, (struct sockaddr*)&cliaddr,&clilen);
			if(connfd < 0)
			{
				if(errno == EINTR)
				{
					continue;
				}
				else
				{
					perror("accept");
				}
			}

			for(i = 0; i< MAX_PROCESS_NUM; ++i)
			{
				if(cptr[i].child_status == 0)
					break;
			}

			if(i == MAX_PROCESS_NUM)
			{
				printf("no available children\n");
				return;
			}

			cptr[i].child_status = 1;
			cptr[i].child_count++;
			navail--;

			n = sendmsg(cptr[i].child_pipefd, "", 1, connfd);
			close(connfd);
			if(--nsel < 0)
			{
				continue;
			}
		}

		for(i = 0; i < MAX_PROCESS_NUM; ++i)
		{
			if(FD_ISSET(cptr[i].child_pipefd, &rset))
			{
				if( (n = readmsg(cptr[i].child_pipefd, buff, MAXLINE)) == 0)
				{
					close(cptr[i].child_pipefd);
					FD_CLR(cptr[i].child_pipefd, &rset);
				}
				cptr[i].child_status = 0;
				++navail;
				if(--nsel <= 0)
				{
					break;
				}
			}
		}
	}
	return 0;
}
