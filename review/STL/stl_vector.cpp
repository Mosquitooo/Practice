
#include <iostream>
using namespace std;
#include <memory.h>

template <class T, class ALLOC = alloc>
class vector
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t      size_type;
	typedef ptrdiff_t   difference_type;
protected:
	typedef simple_alloc<value_type, ALLOC> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storage
	void inser_aux(iterator, const T& x);

	void deallocate()
	{
		if(start)
			data_allocator::deallocate(start, end_of_storage - start);
	}

	iterator allocate_and_fill(size_type n, const T& x)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result,n,x);
		return result;
	}

	void fill_initialize(size_type n, const T& value)
	{
		start = allocate_and_fill(n,value);
		finish = start + n;
		end_of_storage = finish;
	}



public:
	iterator being()
	{
		return start;
	}

	iterator end()
	{
		return finish;
	}

	size_type size() const
	{
		return size_type(end() - being());
	}

	size_type capacity()const
	{
		return size_type(end_of_storage - being());
	}

	bool empty()const
	{
		return begin() == end();
	}

	reference operator[](size_type n)
	{
		return *(begin() + n);
	}

public:
	vector():start(0), finish(0),end_of_storage(0)
	{}

	vecotr(size_type n, const T& value)
	{
		fill_initialize(n, value);
	}

	vecotr(int n, const T& value)
	{
		fill_initialize(n, value);
	}

	vecotr(long n, const T& value)
	{
		fill_initialize(n, value);
	}

	explicit vector(size_type n)
	{
		fill_initialize(n, T());
	}

	~vector()
	{
		destory(start, finish);
		deallocate();
	}
public:
	reference front()
	{
		return *being();
	}
	reference back()
	{
		return *end();
	}

	void push_back(const T& x)
	{
		if(finish == end_of_storage)
			construct(finish, x);
		else
			inser_aux(end(), x);
	}

	void pop_back()
	{
		--finish;
		destory(finish);
	}

	iterator erase(iterator position)
	{
		if(position + 1 != end())
			copy(position + 1, finish, position);
		--finish;
		destory(finish);
		return position;
	}

	void resize(size_type n, const T& x)
	{
		if(n < size())
			erase(begin() + n, end());
		else
			insert(end(), n - size(), x);
	}

	void resize(size_type new_size)
	{
		resize(new_size, T());
	}
	void clear()
	{
		erase(begin(), end());
	}
};