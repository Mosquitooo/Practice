
#include<iostream>
#include<iomanip>
#include<cstring>
using namespace std;


class String
{
	private:
		char* szString;
	public:
		String(char* pstr = NULL);
		String(const String& other);
		~String(void);
		String& operator=(const String& other);
		String operator+(const String& other);
		bool operator==(const String& other);

	public:
		friend ostream& operator<<(ostream&, String&);
		friend istream& operator>>(istream&, String&);		
};

String::String(char* pstr)
{
	cout<<"String:"<<__FILE__<<__LINE__<<endl;
	if(pstr == NULL){
		szString = NULL;
	}
	else{
		szString = new char[strlen(pstr)+1];
		memcpy(szString, pstr, strlen(pstr));
	}
}

String::String(const String& other)
{
	cout<<"String:"<<__FILE__<<__LINE__<<endl;
	int len = strlen(other.szString);
	szString = new char[len+1];
	memcpy(szString, other.szString,len);
}

String::~String()
{
	cout<<"String:"<<__FILE__<<__LINE__<<endl;
	delete this->szString;
}

String& String::operator=(const String& other)
{
	cout<<"String:"<<__FILE__<<__LINE__<<endl;
	if(this == &other)
		return *this;
	delete this->szString;
	int len = strlen(other.szString);
	this->szString = new char[len+1];
	memcpy(this->szString, other.szString,len);
	return *this;	
}

String String::operator+(const String& other)
{
	
}

bool String::operator==(const String& other)
{
	return strcmp(this->szString, other.szString) == 0;
}


ostream& operator<<(ostream& os, String& string)
{
	cout<<"String:"<<__FILE__<<__LINE__<<endl;
	os << string.szString;
	return os;
}

istream& operator>>(istream& is, String& string)
{
	cout<<"String:"<<__FILE__<<__LINE__<<endl;
	char sTempString[255];
	is>>setw(255)>>sTempString;
	string = sTempString;
	return is;
}

int main()
{
	String string1("hello world");
	String *pstring2 = new String("hello");
	cin>>string1;
	cout<<string1<<endl<<*pstring2<<endl;
	return 0;
}




