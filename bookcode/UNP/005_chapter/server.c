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

#define MAXLINE 1023
#define LISTENQ 10
#define PORT    54321
int Socket(int family, int type, int protocol)
{
	int n = 0;
	if((n = socket(family,type,protocol)) < 0)
		perror("socket error");

	return n;
}

void str_echo(int sockfd)
{
	ssize_t n;
	char buff[MAXLINE + 1];

	again:
		while((n = read(sockfd, buff, MAXLINE)) > 0)
		{
			printf("Server: %s\n", buff);
			write(sockfd, buff, n);
			memset(buff, 0x00, MAXLINE);
		}

		if(n < 0 && errno == EINTR)
		{
			goto again;
		}
		else if(n < 0)
		{
			printf("str_echo: read error");
		}
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

int main(int argc, char const *argv[])
{
	int listendfd, connfd;
	pid_t childid;
	socklen_t clilen;
	struct sockaddr_in servaddr, cliaddr;
	char buff[MAXLINE + 1] = {0};
	time_t ticks;

	listendfd = Socket(AF_INET, SOCK_STREAM,0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listendfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(listendfd, LISTENQ);

	signal(SIGCHLD, sig_chld);

	for(;;)
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

		if((childid = fork()) == 0)
		{
			close(listendfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}

	return 0;
}