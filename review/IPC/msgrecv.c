
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
	struct msg_st data;
	long int msg_type;


	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		perror("msgget");
		exit(-1);
	}

	while(running)
	{
		if(msgrcv(msgid, &data, BUFFSIZE, msg_type, 0) == -1)
		{
			perror("msgrecv");
			exit(-1);
		}

		printf("you wrote: %s\n", data.sztext);

		if(strncmp(data.sztext, "end", 3) == 0)
		{
			running = 0;
		}

	}

	if(-1 == msgctl(msgid, IPC_RMID, 0))
	{
		perror("msgctl");
		exit(-1);
	}
	/* code */
	return 0;
}