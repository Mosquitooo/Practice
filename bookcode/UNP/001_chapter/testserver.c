#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

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

int main(int argc, char const *argv[])
{
	int listendfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE + 1] = {0};

	time_t ticks;

	listendfd = Socket(AF_INET, SOCK_STREAM,0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listendfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(listendfd, LISTENQ);

	for(;;)
	{
		connfd = accept(listendfd, NULL,NULL);

		ticks = time(NULL);

		snprintf(buff, MAXLINE, "%.24s\r\n", ctime(&ticks));

		write(connfd, buff, strlen(buff));

		close(connfd);
	}

	return 0;
}