

#include<iostream>
using namespace std;


class base
{
    public:
        base(int a = 0, int b = 0)
        {
            cout<<"  base "<<endl;
        }

        virtual ~base()
        {
            cout<<" ~ base "<<endl;
        }

    private:
        int a;
        int b;

};

class A: public  base
{
    public:
        A()
        {
           cout<<" A"<<endl; 
        }
        ~A()
        {
           cout<<" ~A"<<endl; 
        }
    private:
        int a;
        int b;
};

class B
{
    public:
        B()
        {
           cout<<" B"<<endl; 
        }
        
        ~B()
        {
           cout<<" ~B"<<endl; 
        }
    private:
        int a;
        int b;
};


int main()
{
    base *ba = new A;
    A *pa = dynamic_cast<A*>(ba);

    delete ba;
	return 0;
}




