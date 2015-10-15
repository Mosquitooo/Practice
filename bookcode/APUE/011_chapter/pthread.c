
#include <stdio.h>
#include <pthread.h>

struct foo
{
	/* data */
	int a;
	int b;
	int c;
	int d;
};

void printffoo(const char *s, const struct foo *fp)
{
	printf(s);
	printf("struct add 0x%x\n", (unsigned)fp);
	printf("foo.a: %d\n", fp->a);
	printf("foo.b: %d\n", fp->b);
	printf("foo.c: %d\n", fp->c);
	printf("foo.d: %d\n", fp->d);
}

void* thr_fn1(void *arg)
{
	struct foo foo = {1,2,3,4};
	printffoo("thread 1:", &foo);
	pthread_exit((void*)&foo);
}

void* thr_fn2(void *arg)
{
	printf("thread 2: ID is %x\n", pthread_self());
	printf("thread 2: ID is %d\n", pthread_self());
	pthread_exit((void*)0);
}

int main(int argc, char const *argv[])
{
	int err;
	pthread_t t1, t2;
	struct foo *fp;

	err = pthread_create(&t1, NULL, thr_fn1,NULL);
	if(err != 0)
	{
		perror("pthread_create");
		return 1;
	}
	err = pthread_join(t1, (void*)&fp);
	sleep(1);
	printf("start second thread\n");
	err = pthread_create(&t2, NULL, thr_fn2,NULL);
	if(err != 0)
	{
		perror("pthread_create");
		return 1;
	}
	sleep(1);
	printf("pid: %d\n", getpid());
	printffoo("Parent:\n", fp);
	return 0;
}