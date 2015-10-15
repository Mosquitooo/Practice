
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<list>
#include<stdlib.h>
using namespace std;

class Test
{
	public:
		int m_data;
		char Ch;
	public:
		bool operator < ( const Test& t) const //set sort, must need reload
		{
			return t.m_data > m_data;
		}
		
};

struct Data
{
	int index;
	int data;
};

int main()
{
	cout<< "--------------vector---------------\n"<<endl;
	int a[7] = {1,2,3,4,5,6,7};
	vector <int> vt(a,a+7);

	vector <int>::iterator it = vt.begin();
	vector <int>::iterator ite = vt.end();

	for(;it != ite;it++)
	{
		cout << *it<<" ";
	}
	cout<<endl;
	cout<< "--------------set---------------\n"<<endl;
	list<Data> data_list;
	Data da = {0};
	float i = 0.00000000001;
	da.data = abs(i);


	cout<< "--------------set---------------\n"<<endl;
	set<Test> st;
	Test tt;
	for(int i = 0; i<5;i++)
	{
		tt.m_data = i;
		tt.Ch = 65 + i;
		st.insert(tt);
		
	}
	
	set<Test>::iterator sit = st.begin();
	for(;sit != st.end(); sit++)
	{
		cout<<sit->m_data<<" "<<sit->Ch<<endl;
	}

	cout<< "--------------map---------------\n"<<endl;
	map<int, Test> mt;
	pair<map<int,Test>::iterator,bool> bInsert;
	for(int i = 0; i<5;i++)
	{
		tt.m_data = i;
		tt.Ch = 65 + i;
		//mt.insert(map<int,Test>::value_type(i,tt));
		mt.insert(pair<int,Test>(i,tt));
		//mt[i] = tt;
		if(bInsert.second == true)
		{
			cout<<"insert success"<<endl;
		}
	}
	map<int,Test>::iterator mit = mt.begin();
	for(;mit != mt.end(); mit++)
	{
		cout<<mit->first<<" "<<mit->second.m_data<<" "<<mit->second.Ch<<endl;
	}


}



