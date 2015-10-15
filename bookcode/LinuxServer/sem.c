
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

union semun
{
	int val;
	struct semid_ds* buf;
	unsigned short int* array;
	struct seminfo* __buf;
};

void pv(int semid, int op)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = op;  //信号量操作，1为V操作， -1为P操作
	sem_b.sem_flg = SEM_UNDO; 
	semop(semid, &sem_b, 1);
}


int main()
{
	pid_t ret;
	
	//创建一个信号量并初始化
	int sem_id;
	union semun sem_un;
	sem_id = semget(IPC_PRIVATE, 1,0666); 
	sem_un.val = 1;
	semctl(sem_id, 0, SETVAL, sem_un);
	
	ret = fork();
	if(ret == -1)
	{
		perror("Fork()\n");
		return 1;
	}
	else if(ret == 0)
	{
		pv(sem_id, -1);
		sleep(3);
		printf("Child: pid: %d ppid: %d\n", getpid(), getppid());
		pv(sem_id, 1);
		exit(0);
	}
	else
	{
		pv(sem_id, -1);
		printf("Father: pid: %d ppid: %d\n", getpid(), getppid());
		pv(sem_id, 1);
	}
	
	waitpid(ret, NULL, 0);
	semctl(sem_id, 0, IPC_RMID, sem_un); //删除信号量
	return 0;
}