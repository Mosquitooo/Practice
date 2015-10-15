
//默认构造函数总是在被需要的时候才会被编译产生出来，他并不会为你做啥事
// 1.  当类中含有另一个类对象，并且该对象有默认构造，此时会产生默认构造函数
// 2.  派生自一个有构造函数的基类，此时会产生默认构造函数
// 3.  派生自一个带有虚函数的基类，此时会产生默认构造函数来初始化vptr, 放置适当的vtbl地址

//拷贝构造函数总是在被需要的时候才会被编译产生出来，默认按位拷贝, 以下情况除外
// 1.  类中含有另一个类的对象，且该对象已经定义了拷贝构造
// 2.  类继承自基类，且基类中定义了拷贝构造
// 3.  类中声明了一个或多个虚函数, 很好理解，当发生切割时，必须保证基类的vptr的指向基类的vtbl， 如果是按位拷贝，则会指向子类的vtbl
// 4.  类来自继承串链，其中有一个或多个虚继承.  

//什么情况下使用成员初始化列表
// 1.  初始化一个reference member时
// 2.  初始化一个const member时
// 3.  调用基类的构造函数，并且需要一组参数
// 4.  调用成员对象的构造函数并且需要传一组参数


#include <iostream>
#include <cstring>
using namespace std;

class String
{
public:
	String(const char *s = NULL)
	{
		cout << "String::String() " <<endl;
		if(s == NULL)
		{
			str = NULL;
		}
		else
		{
			str = new char(strlen(s) + 1);
			strcpy(str, s);
		}

	}

	String(const String& s)
	{
		cout << "String::String(String&) " <<endl;
		if(s.str == NULL)
		{
			str = NULL;
		}
		else
		{
			int len = strlen(s.str);
			str = new char(len + 1);
			strcpy(str, s.str);
		}
	}

	String& operator=(const String& s)
	{
		cout << "String::operator=(String&) " <<endl;
		if(this == &s)return *this;

		delete[] str;

		str = new char(strlen(s.str) + 1);
		strcpy(str, s.str);
	}

	~String()
	{
		cout << "String::~String() " << (unsigned long)str<<endl;
		if (str != NULL)
		{
			delete[] str;
			str = NULL;
		}
	}

public:
	char *str;
};

class WordTest1
{
public:
	
	WordTest1(const char* szstr)
	{
		str = new char(strlen(szstr) + 1);
		if(str == NULL) return;
		strcpy(str, szstr);
		len = strlen(szstr) + 1;
	}
	~WordTest1()
	{
		delete[] str;
	}
public:
	char *str;
	int  len;
};

class WordTest2
{
public:
	WordTest2(const char* s):string(s) //使用成员初始化列表, 提高效率。
	{

	}
public:
	String string;
};

int main()
{
	//WordTest1 w1("book");
	//WordTest1 w2 = w1;  //按位拷贝，析构时double free
	//cout<< (unsigned long)w1.str <<endl;
	//cout<< (unsigned long)w2.str <<endl;

	WordTest2 wt1("book");
	WordTest2 wt2 = wt1;
	cout<< (unsigned long)wt1.string.str <<endl;
	cout<< (unsigned long)wt2.string.str <<endl;
}