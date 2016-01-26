
#include <iostream>

void PrintArray(int SrcArr[], int a[], int size)
{
	int i = 0;
	for(i = 0; i < size; ++i)
	{
	printf("%d ", SrcArr[a[i]]);
	}
	printf("\n");
}



int GetInvalidMinIndex(int a[], int size, int CurIndex)
{
	int i = 0;
	for(i = 0; i < size; ++i)
	{
		int j = 0;
		for(j = 0; j < size; ++j)
		{
			if(i == a[j])
			break;
		}
		
		if(j >= size && i >= CurIndex)
		{
			return i;
		}
	}
	return -1;
}



void permutation(int a[], int size)
{
	if(a == 0|| size == 0)
	{
		return;
	}
	
	int *pstack = (int*)malloc(sizeof(int)*size);
	if(pstack == NULL)
	{
		return;
	}	
	
	int i = 0;
	for(i = 0; i < size; ++i)
	{
		pstack[i] = -1;
	}
	
	int CurSize = 0;
	int TopIndex = 0;
	int NextIndex = -1;
	while(pstack[0] < size)
	{
		NextIndex = GetInvalidMinIndex(pstack, size, -1);
		if(NextIndex == -1)
		{
			while(NextIndex == -1)
			{
				pstack[--CurSize] = -1;
				if(CurSize <= 0)
				{
					return;
				}
				NextIndex = GetInvalidMinIndex(pstack, size, pstack[CurSize - 1]);
				if(NextIndex != -1)
				{
					pstack[CurSize - 1] = NextIndex;
				}
			}
		}
		else
		{
			pstack[CurSize++] = NextIndex;
		}

		if(CurSize >= size)
		{
			PrintArray(a, pstack, size);
		}
	}
}

int main()
{
	int a[4] = {1,2,3,4};
	permutation(a, 4);
	
}
