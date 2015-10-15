#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define BUFFSIZE 255

struct msg_st
{
	long int msg_type;
	char sztext[BUFFSIZE];
	/* data */
};

int main(int argc, char const *argv[])
{

	int running = 1;
	int msgid = -1;
	char buff[BUFFSIZE];
	struct msg_st data;

	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		perror("msgget");
		exit(-1);
	}


	while(running)
	{
		fgets(buff, BUFFSIZE, stdin);
		strncpy(data.sztext, buff, BUFFSIZE - 1);

		if(msgsnd(msgid, &data, BUFFSIZE, 0) == -1)
		{
			perror("msgsnd");
			exit(-1);
		}

		if(strncmp(data.sztext, "end", 3) == 0)
		{
			running = 0;
		}
	}

	/* code */
	return 0;
}