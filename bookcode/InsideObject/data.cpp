
//data语意学

//加上多态之后, 编译器将会做如下动作
// 1. 导入一个类的vtbl
// 2. 在每一个类对象中安插vptr
// 3. 加强构造函数，初始话vptr
// 4. 加强析构函数, 抹消指向类虚函数表的vptr

//多重继承
//一个基类指针指向一个多重继承派生对象时,指向如下
// 1. 第一个基类的地址指定操作：指向对象的起始地址.
// 2. 后继基类的地址指定操作：对象起始地址 加上 介于中间的类大小，而针对指向的子类是指针是，需要判断指针是否为空的情况.

//类成员存储
// 1. static成员在类外，存储很直接.
// 2. nonstatic成员存取时通过对象的起始地址加上偏移量. 
// 3. 以 object.x存取时在编译的时候就固定了，但是以pObject->x存取时，由于无法确定指针指向的对象类型，操作将会延迟到执行期.

//对象成员的存取效率
// 1. 单一继承和多重继承不会影响成员的存取效率， 因为成员都是被连续存储于对象中.
// 2. 虚拟继承会产生间接存取操作， 效率将变低

#include <iostream>
using namespace std;

class Point2D
{
public:
	Point2D(float x = 0.0, float y = 0.0):m_x(x),m_y(y)
	{

	};
	float x(){return m_x;}
	float y(){return m_y;}
	void x(float newX){m_x = newX;}
	void y(float newY){m_y = newY;}
	virtual float z(){return 0.0;}
	virtual void z(float newZ){}
	void operator+=(Point2D& rhs)
	{
		m_x = rhs.x();
		m_y = rhs.y();
	}
	~Point2D();

private:
	float m_x;
	float m_y;
};

class Point3D:public Point2D
{
public:
	Point3D(float x = 0.0, float y = 0.0, float z = 0.0):Point2D(x,y),m_z(z)
	{

	};
	virtual float z(){return m_z;}
	virtual void z(float newZ){m_z = newZ;}
	virtual void operator+=(Point2D& rhs)
	{
		Point2D::operator+=(rhs);
		rhs.z();
	}
	~Point3D();

	/* data */
private:
	float m_z;
};

int main()
{
	return 0;
}