

#include <iostream>
#include <string>
using namespace std;

string ReplaceSpace(string str)
{
	string res;
	for(int i = 0; i < str.size(); i++)
		if(str[i] == ' ')res.append("%20");
		else
			res.append(1, str[i]);
	return res;
}

int main(int argc, char const *argv[])
{
	string str(" happy new year ");
	cout << ReplaceSpace(str)<<endl;
	/* code */
	return 0;
}