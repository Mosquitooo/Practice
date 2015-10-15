/************************
//object.cpp 
************************/

#include <iostream>
#include <string>
using namespace std;

typedef void (*func_pointer)(void); 

class Animal
{
public:
	virtual void get_vptr_addr()
	{
		cout<<" Animal::this: "<< this << endl;
	};

	virtual void get_vtbl_addr()
	{
		cout<<" Animal::vtbl:"<< this <<endl;
	};

	void call_first_virtual_func()
	{
		fun = (func_pointer)* ((int*)*(int*)this);
		cout<< "Animal::call_first_virtual_func:" << fun <<endl;
		fun();
	}

public:
	func_pointer fun;
private:
	int age;
	string name;
};

class Bear:public Animal
{
public:
	Bear(){};
	virtual void get_vptr_addr()
	{
		cout<<" Bear::this: "<< this <<endl;
	};

	void call_first_virtual_func()
	{
		fun = (func_pointer)*((int*)*(int*)this);
		cout<< "Bear::call_first_virtual_func:" << ((int*)*(int*)this) <<endl;
		fun();
	}

	void call_second_virtual_func()
	{
		fun = (func_pointer)*((int*)*(int*)this + 2);
		cout<< "Bear::call_second_virtual_func:" << ((int*)*(int*)this + 2) <<endl;
		fun();
	}

};

int main()
{
	//object size:
	//1. nonstatic member data;
	//2. virtual point
	//3. struct align

	Animal a;
	Bear b;
	Animal *pa = &b;
	cout << "point size: "<<sizeof(pa) <<endl;
	cout << "int size: "<<sizeof(int) <<endl;
	cout << "string size: "<<sizeof(string) <<endl;
	cout << "Animal size: "<<sizeof(a) <<endl;

	//polymorphism
	a.get_vptr_addr();
	b.get_vptr_addr();
	a.call_first_virtual_func();
	b.call_first_virtual_func();
	cout<<"vtabl: "<<(int*)&b<<endl;
	cout<<"vtbl fisrt func:"<<((int*)*(int*)&b)<<endl;
	cout<<"vtbl second func:"<<((int*)*(int*)&b + 1)<<endl;
	b.call_second_virtual_func();
	pa->call_first_virtual_func();
}