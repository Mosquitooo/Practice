


#include<stdio.h>
#include<math.h>
int perm(int a[], int nCurIndex, int nSize)
{
	int i = 0;
	static int temp = 0;
	if(nCurIndex == nSize - 1)
	{
		for(i = 0; i< nSize; i++)
			printf("%d ", a[i]);
		printf("\n");
		return 0;
	}

	for(i = nCurIndex; i< nSize ;i++)
	{
		temp = a[i]; a[i] = a[nCurIndex]; a[nCurIndex] = temp;
		perm(a,nCurIndex+1,nSize);
		temp = a[i]; a[i] = a[nCurIndex]; a[nCurIndex] = temp;
	}
	return 0;
}

int biteq1(unsigned int a)
{
	int i = 0;
	int count = 0;
	while(i < 32)
	{
		if((a>>i) & 0x01 == 1)
		count++;
		i++;
	}
	return count;
}

int comb(int a[], unsigned int nSize, int nSelectNum)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int ret = pow(2, nSize);
	for(i = 0; i < ret; i++)
	{
		if(biteq1(i) == nSelectNum)
		{
			for( j = 0; j < nSize; j++)
			{
				if((i>>j) & 0x01 == 1)
					printf("%d ", a[j]);
			}
			printf("\n");
		}
	}
	return 0;
}

int main()
{
	int i = 0;
	int a[4] = {1,2,3,4};
	perm(a,0,4);
	for(i = 1; i <= 4;i++)
	comb(a, 4, i);
}






