
#include<iterator>
#include<iostream>
using namespace std;

class Array
{
public:
	static int RemoveDupelicates(int array[], int size);
	static int RemoveDupelicates(int array[], int size, int nRepeatTimes);
	static int SearchInRotatedArray(int array[], int size, int target);

};

//the array must be sorted
int Array::RemoveDupelicates(int array[], int size, int nRepeatTimes)
{
	if(size == 0)
		return 0;
	
	int index = 0;
	int occur = 1;
	
	for(int i = 0; i < size; ++i)
	{
		if(array[index] != array[i])
		{
			array[++index] = array[i];
			occur = 1;
		}
		else
		{
			if(occur < nRepeatTimes)
			{
				array[++index] = array[i];
				++occur;
			}
		}
	}
	return index + 1;
}

int Array::RemoveDupelicates(int array[], int size)
{
	return distance(array, unique(array, array + size));
}

int Array::SearchInRotatedArray(int array[], int size, int target)
{
	if(size <= 0 || array == NULL)
	{
		return -1;
	}
	int left = 0;
	int right = size - 1;
	int middle = 0;
	
	while(left <= right)
	{
		middle = (left + right)/2;
		cout << array[left] <<" "<<array[middle] << " " <<array[right]<<endl;
		if(target == array[middle])
		{
			return middle;
		}
		
		if(target < array[middle])
		{
			if(target >= array[left])
			{
				right = middle - 1;
			}
			else
			{
				left = middle + 1;
			}
		}
		
		if(target > array[middle])
		{
			if(target <= array[right])
			{
				left = middle + 1;
			}
			else
			{
				right = middle - 1;
			}
		}
	}
	return -1;
}


int main()
{
	int a[] = {7,1,2,3,4,5,6};
	
	for(int i = 1; i <= sizeof(a)/sizeof(a[0]); ++i)
	{
		cout << Array::SearchInRotatedArray(a, sizeof(a)/sizeof(a[0]), i) <<endl;
	}

	return 0;
}
