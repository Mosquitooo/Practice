
#include <stdio.h>

int main()
{
	system("stty -echo -icanon");
	while(1)
	{
		printf("1");
		fflush(stdout);
	}
	//printf("\033[2J");
	//printf("\033[47;31mhello world\033[5m");
}
