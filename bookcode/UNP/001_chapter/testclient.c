#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 1023
#define PORT 	33333

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

	int count = 0;
	while((n = read(sockfd, recvline, MAXLINE)) > 0)
	{
		recvline[n] = 0;
		if(fputs(recvline, stdout) == EOF)
		{
			perror("fputs error");
		}
		++count;
	}

	if(n < 0)
	{
		perror("read error");
	}

	printf("count: %d\n", count);

	return 0;
}