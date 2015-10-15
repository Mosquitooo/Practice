
#include "stdio.h"

struct S
{ 
    int a;
    int b; 
};

int all(int a[], int size, int ncur)
{
	int i =0;
	int nTemp;
	if(ncur == size - 1)
	{
		for(i = 0; i < size; i++)
		{
			printf(" %d ", a[i]);
		}
		printf("\n");
		return;
	}
	
	for(i = ncur; i< size; i++)
	{
		nTemp = a[i]; a[i] = a[ncur]; a[ncur] = nTemp;
		all(a,size,ncur+1);
		nTemp = a[i]; a[i] = a[ncur]; a[ncur] = nTemp;
	}
	return 0;
}

void TEST()
{
  int x;
    scanf("%d", &x);
	if (x-- < 5)
		printf("% d", x);
    else
		printf("%d", x++);
}
	  
main( )
{ 
    struct S a, *p=&a;
    a.a = 99;
    int array[3] = {1,2,3};
	all(array,3,0);
	printf("%d", 93&-8);
}




