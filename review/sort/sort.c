

#include<stdio.h>


void Sort_Quick(int *a, int size)
{
	int i = 0;
	int j = size - 1;
	int key = a[i];
	if(a == NULL)
		return;
	if(size <= 1)
		return;
	while(i < j)
	{
		while(i < j && a[j] >= key)j--;
		a[i] = a[j];
		while(i < j && a[i] <= key)i++;
		a[j] = a[i];
	}
	a[i] = key;

	int n = 0;
	for(; n < size; n++)
		printf("%d ", a[n]);
	printf("\n");
	printf("i : %d  size: %d\n", i, size);
	//Sort_Quick(a, i);
	Sort_Quick((a + i + 1), size - i - 1);
}


struct A
{
	int a;
	short b;
	int c;
	char d;
};

struct B
{
	double a;
	short b;
	int c;
	char d;
};


int main()
{
	printf("A: %d B: %d\n",sizeof(struct A), sizeof(struct B));
	int a[] = {57,68,59,52,72,28,96,33,24};
	Sort_Quick(a, sizeof(a)/sizeof(a[0]));
}












