
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define LISTEN_NUMBER 5
#define MAXPOLL       10000
#define MAXSIZE       1024
#define PORT          54321

int setnonblocking(int fd)
{
	if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		perror("fcntl:");
		return -1;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	int sockfd;

	struct sockaddr_in servaddr;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd <= 0)
	{
		perror("socket: ");
		exit(0);
	}

	if(setnonblocking(sockfd) == -1)
	{
		printf("etnonblocking error\n");
		return -1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("socket: ");
		exit(0);
	}

	if(listen(sockfd, LISTEN_NUMBER) == -1)
	{
		perror("socket: ");
		exit(0);
	}

	int epoll_fd;
	epoll_fd = epoll_create(MAXPOLL);

	struct epoll_event  ev;
	ev.events =  EPOLLIN | EPOLLET;
	ev.data.fd = sockfd; 
	if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD, sockfd, &ev) < 0)
	{
		printf("epoll_ctl error\n");
		return -1;
	}

	int number = 0;
	int cur_fd_number = 1;
	int connfd = 0;
	struct epoll_event evs[MAXPOLL]; 
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	char buff[MAXSIZE] = {'\0'};
	for(;;)
	{
		if((number = epoll_wait(epoll_fd, evs, cur_fd_number, 0)) == -1)
		{
			perror("epoll_wait");
			return 0;
		}

		int i = 0;
		for(i = 0; i < number; ++i)
		{
			if((evs[i].data.fd == sockfd) && (cur_fd_number < MAXPOLL))
			{
				if((connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &addrlen)) == -1)
				{
					printf("error: %d  %s\n", errno, strerror(errno));
					if(errno == EINTR)
					{
						continue;
					}
					else
					{
						exit(0);
					}
				}

				ev.events =  EPOLLIN | EPOLLET;
				ev.data.fd = connfd; 
				if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD, connfd, &ev) < 0)
				{
					printf("epoll_ctl error\n");
					return -1;
				}
				++cur_fd_number;
			}
			else
			{
				int n = 0;
				if( (n = read(evs[i].data.fd, buff, MAXSIZE)) <= 0)
				{
					close(evs[i].data.fd);
					epoll_ctl(epoll_fd,EPOLL_CTL_DEL, evs[i].data.fd, &ev);
					--cur_fd_number;
					continue;
				}

				printf("Client: %s\n", buff);
				write(evs[i].data.fd, buff, n);
			}
		}
	}
	return 0;
}