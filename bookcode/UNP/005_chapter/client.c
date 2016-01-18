#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 1023
#define PORT 	33333

void str_cli(FILE* fp, int sockfd)
{
	int maxfd;
	fd_set rset;
	char sendline[MAXLINE+1], recvline[MAXLINE+1];

	for(;;)
	{
		//注意不要把rset看作是简单的一个变量
		//注意它其实是可以包含一组套接字的哦，  
		//相当于是封装的数组！每次都要是新的哦
		//所以要写在循环里面

		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfd = fileno(fp)> sockfd? fileno(fp):sockfd;

		select(maxfd+1, &rset, NULL,NULL,NULL);

		if(FD_ISSET(sockfd, &rset))
		{
			if(read(sockfd, recvline, MAXLINE) == 0)
			{
				perror("read error");
				exit(0);
			}
			if(strlen(recvline) > 0)
				printf("Server: %s\n", recvline + 4);
			memset(recvline, 0x00, MAXLINE);
		}

		if(FD_ISSET(fileno(fp), &rset))
		{
			char buffer[256];
			memset(sendline, 0x00, sizeof(sendline));
			memset(buffer, 0x00, sizeof(buffer));
			if(fgets(sendline, MAXLINE, fp) == NULL)
			{
				return;
			}
			
			int len = strlen(sendline);
			memcpy(buffer, &len, sizeof(len));
			memcpy(buffer + sizeof(len), sendline, sizeof(sendline));
			write(sockfd, buffer, strlen(sendline) + 4);
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