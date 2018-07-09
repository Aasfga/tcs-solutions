//
// Created by Dominik Grybos on 16.01.2017.
//

#ifndef BETA_SOLUTION_H
#define BETA_SOLUTION_H

#include <iostream>

using namespace std;

class vector
{
	string *array;
	int maxSize;
	int crrSize;


	void resize()
	{
		maxSize *= 2;
		string *newArray = new string[maxSize];

		for(int i = 0; i < crrSize; i++)
		{
			newArray[i] = array[i];
		}

		delete[] array;
		array = newArray;
	}

public:


	vector(int a = 4, int b = 0, string c = "")
	{
		array = new string[a];
		maxSize = a;
		crrSize = b;


		int maxI = b < a ? b : a;
		for(int i = 0; i < maxI; i++)
		{
			array[i] = c;
		}
	}

	vector(const vector &aVector)
	{
		crrSize = aVector.crrSize;
		maxSize = aVector.maxSize;
		array = new string[maxSize];


		for(int i = 0; i < crrSize; i++)
		{
			array[i] = aVector.array[i];
		}
	}

	void resize(int n)
	{
		while(n > maxSize)
		{
			resize();
		}



		for(int i = crrSize; i < n; i++)
		{
			array[i] = "";
		}

		crrSize = n;
	}

	bool isFull()
	{
		return crrSize == maxSize;
	}

	void clear()
	{
		crrSize = 0;
	}

	int getSize()
	{
		return crrSize;
	}

	void add(string x)
	{
		if(isFull())
			resize();

		array[crrSize++] = x;
	}

	string& operator[](int i)
	{
		if(i >= crrSize)
		{
			if(i == 0)
				return array[0];
			else
				return array[crrSize - 1];
		}

		return array[i];
	}

	void insert(int index, string x)
	{
		if(index > crrSize)
			return;
		if(isFull())
			resize();


		for(int i = crrSize; i > index; i--)
		{
			array[i] = array[i -1];
		}

		array[index] = x;
		crrSize++;
	}

	void erase(int index)
	{
		if(index >= crrSize)
			return;

		crrSize--;

		for(int i = index; i < crrSize; i++)
		{
			array[i] = array[i + 1];
		}
	}

	void operator=(const vector &aVector)
	{
		if(this == &aVector)
			return;

		delete [] array;
		crrSize = aVector.crrSize;
		maxSize = aVector.maxSize;
		array = new string[maxSize];

		for(int i = 0; i < crrSize; i++)
			array[i] = aVector.array[i];
	}


	friend ostream& operator<<(ostream &out, const vector &aVector);
	friend istream& operator>>(istream &in, const vector &aVector);


	~vector()
	{
		delete [] array;
	}
};


ostream& operator<<(ostream &out, const vector &aVector)
{
	for(int i = 0; i < aVector.crrSize; i++)
		out<< aVector.array[i]<< " ";

	return out;
}

istream& operator>>(istream &in, const vector &aVector)
{
	for(int i = 0; i < aVector.crrSize; i++)
	{
		in>> aVector.array[i];
	}

	return in;
}

#endif //BETA_SOLUTION_H
