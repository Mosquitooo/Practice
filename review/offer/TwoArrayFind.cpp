

#include <iostream>
#include <vector>
using namespace std;


bool find(vector<vector<int> > array, int target)
{
	int i = 0;
	int j =  array[i].size() - 1;
	while(i < array.size() && j >= 0)
	{
		if(array[i][j] > target){
			j--;
		}
		else if(array[i][j] < target){
			i++;
		}
		else
			return true;
	}
	return false;
}

int main(int argc, char const *argv[])
{
	vector<vector<int> > array(4);
	for(int i = 0; i < array.size(); i++){
		array[i].resize(4);
	}

	for(int i = 0; i < array.size(); i++)
	{
		for(int j = 0; j < array[i].size(); j++){
			array[i][j] = i + 2*j + 1;
			cout<<array[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;


	cout << find(array, 0) <<endl;
	/* code */
	return 0;
}