//================================================================================================================
// MemoryMgr.h
//
//================================================================================================================
// History
//
// Created on 10/10/2014 by Dustin Watson
//================================================================================================================
#ifndef __MEMORYMGR_H
#define __MEMORYMGR_H
//================================================================================================================
//================================================================================================================
#define PRINT_COMMENTS 0
//================================================================================================================
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
using namespace std;
//================================================================================================================
extern ofstream pfile;
//================================================================================================================
//================================================================================================================
struct MemRef
{
	int ref;
	string who;
	MemRef() : ref(0), who("Anonomous")
	{
	}
};
//================================================================================================================
template <typename T>
class SmartPointer
{
public:
	
	SmartPointer(string name="Anonomous")
	{
		__from = name;
		__name = name;
		__reference = 0;
		__data = NULL;
		//__data.resize(1);
	}
	
	SmartPointer(T* pValue, string name="Anonomous")
	{
		//__data.resize(1);
		__from = name;
		__name = name;
		if (PRINT_COMMENTS == 1) pfile << "------ Memory Created" << endl;
		__reference = 0;
		__reference++;
		__data = pValue;
	}
	
	// Copy the data
	SmartPointer(const SmartPointer<T>& sp) : __data(sp.__data)
	{
		if (PRINT_COMMENTS == 1) pfile << "------ Memory Copied" << endl;
		__from = sp.__from;
		__name = "Copy";
		__reference = sp.__reference;
		__reference++;
	}
	
	~SmartPointer()
	{
		if (PRINT_COMMENTS == 1) pfile << "------ Enter Destructor for " << __name << endl;
		
		if (!ReleaseMem() && PRINT_COMMENTS == 1)
		{
			pfile << "------  Could not Delete Memory from " << __name << endl;
			pfile << "------  because Memory still occupied by " << __from << endl;
		}
	}
	
public:

	void IncReference() { __reference++; }
	
	void SetName(string name) { __name = name; }
		
	operator T*() { return __data; }
	
	//T* operator & () const
	//{
	//	return &__data;
	//}
	
	bool operator == (const T*& sp)
	{
		return __data == sp;
	}
	
	bool operator != (const T*& sp)
	{
		return !(__data == sp);
	}

	bool operator ! ()
	{
		return !(__data);
	}
	
	T& operator * ()
	{
		return *__data;
	}
	
	T* operator -> ()
	{
		return __data;
	}
	
	void InitMem(int count)
	{
		memset(__data, 0, sizeof(T) * count);
	}

	void MemCopy(void* data, int count)
	{
		memset(__data, data, sizeof(T) * count);
	}

	T* Data()
	{
		return __data;
	}
	
	void insert(T* sp)
	{
		__data = sp;
		__reference++;
	}
	
	SmartPointer<T>& operator = (const SmartPointer<T>& sp)
	{
		if (this != &sp)
		{
			if (PRINT_COMMENTS == 1) pfile << "------ Assignment 1" << endl;
			
			//ReleaseMem();
			
			__from = sp.__from;
			__data = sp.__data;
			__reference = sp.__reference;
			__reference++;
		}
		
		return *this;
	}
	
	SmartPointer<T>& operator = (T* data)
	{
		if (__data != data)
		{
			if (PRINT_COMMENTS == 1) pfile << "------ Assignment 2" << endl;
			
			//ReleaseMem();
			
			MemRef mr = data->reference;
			__data = data;
			__reference = mr.ref;
			__from = mr.who;
			__reference++;
			mr.ref++;
			data->reference = mr;
		}
		
		return *this;
	}
	
	bool ReleaseMem()
	{
		--__reference;
		if (__reference == 0)
		{
			if (PRINT_COMMENTS == 1) pfile << "------ Memory Deleted for " << __name << endl;
			delete __data;
			__data = NULL;
			return true;
		}
		
		return false;
	}
	
protected:
	
	T* __data;
	int __reference;
	
	string __name;
	
	//Who occupies the memory
	string __from;
};
//================================================================================================================
template <typename T, size_t S>
class SmartStack
{
public:
	
	SmartStack()
	{
		__data = new SmartPointer<T>[S];
		__size = S;
		__index = 0;
	}
	
	SmartStack(const SmartStack<T, S>& st)
	{
		copy(st);
	}
	
	~SmartStack()
	{
		ReleaseMem();
	}
	
public:
	
	void push(T* data, string name="Anonomous")
	{
		__data[__index] = data;
		__data[__index].SetName(name);
		__index++;
	}
	
	T* pop() { return __data[--__index]; }
	
	T* top() { return __data[__index]; }
	
	virtual bool ReleaseMem()
        {
		delete [] __data;
		__data = NULL;
		__index = 0;
		__size = 0;
        }
	
	void shuffle()
	{
		srand(0);
		for (int i = __size - 1; i > 1; i--)
		{
			int j = rand() % i;
			swap(__data[j], __data[i]);
		}
	}
	
	void swap(T* r, T* s)
	{
		T tmp = *r;
		*r = *s;
		*s = tmp;
	}
	
	int size() { return __size; }
	int count() { return __index; }
	
	SmartStack<T, S>& operator = (const SmartStack<T, S>& st)
	{
		if (this != &st)
		{
			copy(st);
		}
		
		return *this;
	}
	
private:
	
	void copy(const SmartStack<T, S>& st)
	{
		__data = new SmartPointer<T>[st.__size];
		__index = st.__index;
		__size = st.__size;
		for (int i = 0; i < __size; i++)
			__data[i] = st.__data[i];
	}
	
	SmartPointer<T>* __data;
	int __index;
	int __size;
};
//================================================================================================================
template <typename T>
class SmartArray2D
{
public:
	
	SmartArray2D() : __width(0), __height(0)
	{
	}
	
	SmartArray2D(size_t x, size_t y)
	{
		resize(x, y);
	}
	
	//Copy the data
	SmartArray2D(const SmartArray2D<T>& sp) : __data(sp.__data)
	{
	}
	
	//Reset the array width and height
	void resize(size_t x, size_t y)
	{
		__width = x;
		__height = y;
		
		__data.resize(x, vector<T>(y));
	}
	
	size_t width()
	{
		if (__width == 0)
			;//Error throw exception
		return __data.size();
	}

	size_t height()
	{
		if (__height == 0)
			;//Error throw exception
		return __data[0].size();
	}
	
	vector<T> InnerVector(size_t x)
	{
		if (__height == 0)
			;//Error throw exception
		return __data[x];
	}
	
	T& operator () (size_t x, size_t y)
	{
		if (__width == 0 && __height == 0)
			;//Error throw exception
		return __data[x][y];
	}
	
	void insert(size_t x, size_t y, T value)
	{
		if (__width == 0 && __height == 0)
			;//Error throw exception
		__data[x][y] = value;
	}
	
	bool operator == (const SmartArray2D<T>& other)
	{
		return __data == other.__data;
	}

	bool operator != (const SmartArray2D<T>& other)
	{
		return !(__data == other.__data);
	}
	
	SmartArray2D<T>& operator = (const SmartArray2D<T>& other)
	{
		if (this != &other)
		{
			__data = other.__data;
		}

		return *this;
	}
	
private:

	size_t __width;
	size_t __height;
	
	vector< vector<T> > __data;
};
//================================================================================================================
#endif//__MEMORYMGR_H
