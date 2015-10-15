
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

#define MAX_SIZE 1023

int my_open(char* pathname, int mode)
{
	int fd = -1;
	int sockfd[2];
	int status;

	pid_t childid;

	char c;
	char argsockfd[10];
	char argmode[10];

	socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);

	if((childid = fork()) == 0)
	{
		close(sockfd[0]);
		snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
		snprintf(argmode, sizeof(argmode), "%d", mode);

		execl("./openfile", "openfile", argsockfd, pathname, argmode , NULL);
		perror("execl error: ");
	}

	close(sockfd[1]);

	waitpid(childid, &status, 0);
	if(WIFEXITED(status) == 0)
	{
		perror("child did not terminate:");
		exit(0);
	}
	if((status = WEXITSTATUS(status)) == 0)
	{
		Read_fd(sockfd[0], &c, 1, &fd);
	}
	else
	{
		errno = status;
		fd = -1;
	}
	close(sockfd[0]);
	return(fd);
}

int main(int argc, char const *argv[])
{
	int fd = -1;
	int n = 0;

	char buff[MAX_SIZE + 1] = {0};

	if(argc != 2)
	{
		printf("mycat <pathname>\n");
		return 0;
	}

	//如果把my_open改成open程序则只是把一个文件内容复制到标准输出
	if((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open:");
		exit(0);
	}

	while((n = read(fd, buff, MAX_SIZE)) > 0)
	{
		write(STDERR_FILENO, buff, n);
	}
	printf("\n");
	return 0;
}