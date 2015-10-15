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
#define PORT    33000
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

int main(int argc, char const *argv[])
{
	int listendfd, connfd;
	int maxfd;
	int clilent[FD_SETSIZE];
	fd_set rset, allset;
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

	maxfd = listendfd;
	for(;;)
	{
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL,NULL,NULL);
		if(FD_ISSET(listendfd))
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

			for(i = 0; i< FD_SETSIZE; ++i)
			{
				if(clilent[i] < 0)
					clilent[i] = connfd;
				break;
			}

			if(i == FD_SETSIZE)
			{
				printf("too many clients\n");
			}

			FD_SET(connfd, &allset);

			if(connfd > maxfd)
				maxfd = connfd;

			if(i > maxi)
				maxi = i;

			if(--nready < 0)
				continue;

		}

		for(i = 0; i < maxfd; ++i)
		{
			if( (sockfd = clilent[i]) < 0)
				continue;
			if(FD_ISSET(sockfd, &rset))
			{
				if( (n = read(sockfd, buff, MAXLINE)) == 0)
				{
					close(sockfd);
					FD_CLR(sockfd, &allset);
					clilent[i] = -1;
				}
				else
				{
					write(sockfd, buff, n);
				}

				if(--nready <= 0)
				{
					break;
				}
			}
		}
	}

	return 0;
}