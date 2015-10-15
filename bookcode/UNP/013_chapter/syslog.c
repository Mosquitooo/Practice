

#include <syslog.h>

int main(int argc, char const *argv[])
{
	while(1)
	{
		sleep(1);
		syslog(LOG_INFO|LOG_USER, "hello world %d", 110);
	}

	/* code */
	return 0;
}