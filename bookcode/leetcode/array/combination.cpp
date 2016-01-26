
#include <iostream>

void PrintArray(int srcArr[], int a[], int size)
{
	int i = 0;
	for(i = 0; i < size; ++i)
	{
		printf("%d ", srcArr[a[i]]);
	}
	printf("\n");
}

void combination(int a[], int size, int n)
{
	if(a == NULL || size == 0 || n == 0)
	{
		return;
	}
	
	int *pcombi = (int*)malloc(sizeof(int)*n);
	if(pcombi == NULL)
	{
		return;
	}
	
	int nCurSize = 0;
	int nCurTopIndex = 0;
	
	int i = 0;
	for(i = 0; i < n; ++i)
	{
		pcombi[i] = -1;
	}
	
	while(pcombi[0] < size - n)
	{
		while(nCurSize < n && nCurTopIndex < size)
		{
			pcombi[nCurSize] = nCurTopIndex;
			++nCurSize;
			++nCurTopIndex;
		}
		
		if(nCurSize == n)
		{
			PrintArray(a, pcombi, nCurSize);
		}
		--nCurSize;
		nCurTopIndex = pcombi[nCurSize];
		pcombi[nCurSize] = -1;
		++nCurTopIndex;
	}
	
	free(pcombi);
}

int main()
{
	int a[7] = {1,2,3,4,5,6,7};
	combination(a, 3, 3);
	combination(&a[4], 3, 3);
	combination(a, 7, 0);
	combination(a, 5, 4);
	
}
