

#include "stdio.h"

typedef struct
{
	int data;
	struct Node* pLeft;
	struct Node* pRight;
}Node;

int Tree_Insert(Node **Seed, int data)
{
	if(*Seed == NULL)
	{	
		Node *pNode = (Node*)malloc(sizeof(Node));
		pNode->data = data;
		pNode->pLeft = NULL;
		pNode->pRight = NULL;
		*Seed = pNode;
	}
	else if(data < (*Seed)->data)
	{
		Tree_Insert((&(*Seed)->pLeft), data);
	}
	else
	{
		Tree_Insert((&(*Seed)->pRight), data);
	}
	return 0;
}

int Tree_Delete(Node* Seed, int KeyValue)
{
	
}

int Tree_Travel(Node* Seed)
{
	if(Seed != NULL)
	{
		Tree_Travel(Seed->pLeft);
		printf("%d ", Seed->data);
		Tree_Travel(Seed->pRight);
	}
	return 0;
}

#define MAX_NUMBER 64

int Tree_Travel_Test(Node * Seed)
{
	Node *Stack[MAX_NUMBER];
	Node *p = NULL;
	int top = -1;
	if(Seed != NULL)
	{
		top++;
		Stack[top] = Seed;
		while(top > -1)
		{
			p = Stack[top];
			top--;
			printf("%d ", p->data);

			if(p->pLeft != NULL)
			{
				top++;
				Stack[top] = p->pLeft;
			}
			
			if(p->pRight != NULL)
			{
				top++;
				Stack[top] = p->pRight;
			}
		}
		printf("\n");
	}
	return 0;
}

int Tree_Max_Min(Node* Seed)
{
        static int nMax = 0;
        static int nMin = 0;      
    if(Seed != NULL)
   {
        if(nMax < Seed->data)
        {
           nMax = Seed->data; 
        }
       if(nMin > Seed->data)
        {
            nMin = Seed->data;
       }
       Tree_Max_Min(Seed->pLeft);
        Tree_Max_Min(Seed->pRight);
	return abs(nMax - nMin);
    }
}

int Match_Max_String(char *pKey, char* pString)
{
	if(pKey == NULL || pString == NULL)
		return -1;
	int nKeySize = strlen(pKey);
	int nStringLen = strlen(pString); 
	int i =0, j=0;
	while(pKey[i] == pString[j] && i < nStringLen)
	{
		i++;
		j++;
	}
}

int main()
{
	int i = 0;
	int a[7] = {1,2,5,5,8,3,0};
	Node *pTree = NULL;
	for(i = 0; i< 7; i++)
	{
		Tree_Insert(&pTree, a[i]);
	}
	Tree_Travel_Test(pTree);
	printf("\n");
		printf("%d\n",Tree_Max_Min(pTree));

}



