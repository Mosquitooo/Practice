#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 1023
#define PORT 	33000

void str_cli(FILE* fp, int sockfd)
{
	int maxfd;
	fd_set rset;
	char sendline[MAXLINE+1], recvline[MAXLINE+1];

	while((fgets(sendline, MAXLINE, fp)) != NULL)
	{
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);

		maxfd = max(fileno(fp), sockfd) + 1;
		select(maxfd+1, &rset, NULL,NULL,NULL);

		if(FD_ISSET(sockfd, &rset))
		{
			if(read(sockfd, recvline, MAXLINE) == 0)
			{
				perror("fputs error");
				exit(0);
			}
			printf("client: %s\n", recvline);
			memset(recvline, 0x00, MAXLINE);
		}
		if(FD_ISSET(fileno(fp), &rset))
		{
			if(fgets(sendline, MAXLINE, fp) == NULL)
			{
				return;
			}
			write(sockfd, sendline, strlen(sendline));
		}
	}
}

int main(int argc, char const *argv[])
{
	int sockfd, n;
	char recvline[MAXLINE + 1];

	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		printf("usage: a.out <IP>");
		return 0;
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		printf("errno is: %d\n",errno);
		perror("socket error");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);	//daytime server


	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		printf("inet_pton: %s", argv[1]);
		return 0;
	}

	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect error");
	}

	str_cli(stdin, sockfd);
	return 0;
}