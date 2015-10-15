

#include <iostream>
#include <vector>
using namespace std;



int MinFind(vector<int> &array)
{
	if(array.size() == 0)
		return -1;
	if (array.size() == 1)
	{
		return array[0];
	}
	int start = 0;
	int end   = array.size() - 1;
	int mid   = 0;
	while(array[end] <= array[start])
	{
		if(end == start + 1)
		{
			mid = end;
			break;
		}
		mid = start + (end - start)/2;
		if(array[mid] <= array[end])
			end = mid;
		else
			start = mid;

	}
	return array[mid];

}


int main(int argc, char const *argv[])
{
	vector<int> a(2);
	a[0] = 3;
	a[1] = 4;
	//a[2] = 5;
	//a[3] = 2;
	//a[4] = 2;

	cout<< MinFind(a)<<endl;
	/* code */
	return 0;
}