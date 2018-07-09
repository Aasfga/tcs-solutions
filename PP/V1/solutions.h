//
// Created by Dominik Grybos on 04.01.2017.
//

#ifndef V_MAIN_H_H
#define V_MAIN_H_H

#include <iostream>

using namespace std;

class point
{
	int x;
	int y;

public:

	point(int x = 0, int y = 0)
	{
		this -> x = x;
		this -> y = y;
	}

	int getX()
	{
		return x;
	}

	int getY()
	{
		return y;
	}

	void print()
	{
		cout<< x<< " "<< y<< "\n";
	}
};

class stack
{
	long long *array;
	int capacity;
	int size;

public:

	stack(int cap = 15)
	{
		capacity = cap;
		size = 0;
		array = new long long[capacity];
	}

	bool full()
	{
		return  size == capacity;
	}

	bool empty()
	{
		return size == 0;
	}

	void push(long long x)
	{
		if(!full())
			array[size++] = x;
	}

	long long top()
	{
		if(!empty())
			return array[size - 1];

		return 0;
	}

	long long pop()
	{
		if(!empty())
			return array[--size];

		return 0;
	}

	void clear()
	{
		size = 0;
	}

	void resize(int n)
	{
		if(n <= capacity)
			return;


		long long *nArray = new long long[n];
		for(int i = 0; i < size; i++)
			nArray[i] = array[i];

		delete array;
		array = nArray;
		capacity = n;
	}

	void print()
	{
		for(int i = 0; i < capacity; i++)
			cout<< array[i] << "\n";
	}


	~stack()
	{
		delete [] array;
	}
};

class queue
{
	int capacity;
	int begin;
	int size;
	point *array;

public:


	queue(int cap = 15)
	{
		begin = 0;
		size = 0;
		capacity = cap;
		array = new point[capacity];
	}

	bool empty()
	{
		return size == 0;
	}

	bool full()
	{
		return size == capacity;
	}

	void clear()
	{
		size = 0;
		begin = 0;
	}

	void enqueue(point &x)
	{
		if(full())
			return;

		array[(begin + size) % capacity] = x;
		size++;
	}

	point dequeue()
	{
		if(empty())
		{
			point zero;
			return zero;
		}


		point ans = array[begin];
		begin++;
		begin %= capacity;
		size--;
		return ans;
	}

	point front()
	{
		if(empty())
		{
			point zero;
			return zero;
		}


		return array[begin];
	}

	void resize(int n)
	{
		if(n <= capacity)
			return;
		point* nArray = new point[n];

		for(int i = 0; i < size; i++)
		{
			nArray[i] = array[(begin + i) % capacity];
		}

		capacity = n;
		begin = 0;
		delete [] array;
		array = nArray;

	}

	~queue()
	{
		delete [] array;
	}

	void print()
	{

		for(int i = 0; i < capacity; i++)
			array[i].print();

		cout<< "Begin: " << begin << "\nSize: "<< size << "\n";
	}
};

#endif //V_MAIN_H_H
