
#include<iostream>
using namespace std;

// 私有属性无法被对象访问且被继承
// 保护属性无法被对象访问但是可继承

class Base
{
public:
	Base()
	{
		pub = 0;
		pri = 0;
		pro = 0;
	}
public:
	int pub;
private:
	int pri;
protected:
	int pro;
public:
	void Func()
	{
		
	}
};

class Driver:public Base
{

public:
	void Func()
	{
		cout << pro <<endl;
	}
};

int main()
{
	Base base;
	//cout << base.pro <<endl; //error! protected
	
	Driver driver;
	//cout << driver.pro <<endl; //error! protected
	
}
