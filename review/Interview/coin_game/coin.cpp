

#include<stdio.h>

int getWinValue(int a[], int size)
{
	int i = 0;
	int j = size - 1;
	int count = 0;
	int sum1 = 0;
	int sum2 = 0;
	int nIndex = 0;
	while(i !=j)
	{
		printf("i: %d j:%d\n", i,j);
		if(a[i] >= a[j])
		{
			nIndex =	i++;
		}
		else
		{
			nIndex =	j--;
		}
		
		if(count % 2 == 0)
		{
			sum1 += a[nIndex];
		}
		else
		{
			sum2 += a[nIndex];
		}
		count++;
	}
	return (sum1 > sum2?sum1:sum2);
	
}

int main()
{
	int a[4] = {1,9,1};
	printf("Win Value: %d\n", getWinValue(a,sizeof(a)/sizeof(a[0])));
}







