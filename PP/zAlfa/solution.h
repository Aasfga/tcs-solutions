//
// Created by Dominik Grybos on 04.02.2017.
//

#ifndef ALFA_SOLUTION_H
#define ALFA_SOLUTION_H
using namespace std;

#include <iostream>
#include <string>

using namespace std;

class number
{
	int *array;
	int size;
	int minus;


public:

	bool is_zero()
	{
		return size == 1 && array[0] == 0;
	}

	void subFormat()
	{
		for(int i = 0; i < size; i++)
		{
			if(array[i] < 0)
			{
				if(array[i + 1] == 1 && i + 2 == size)
					size--;
				array[i] += 10;
				array[i + 1] -= 1;
			}
		}

		while(size > 1 && array[size - 1] == 0)
			size--;

	}

	void addFormat()
	{
		for(int i = 0; i < size; i++)
		{
			if(array[i] >= 10)
			{
				if(i + 1 == size)
					size++;
				array[i + 1] += array[i] / 10;
				array[i] %= 10;
			}
		}

		while(size > 1 && array[size - 1] == 0)
			size--;
	}

	void multFormat()
	{
		for(int i = 0; i < size; i++)
		{
			if(array[i] >= 10)
			{
				if(i + 1 == size)
					size++;
				array[i + 1] += array[i] / 10;
				array[i] %= 10;
			}
		}

		while(size > 1 && array[size - 1] == 0)
			size--;
	}

	void setZero()
	{
		for(int i = 0; i < 300; i++)
			array[i] = 0;
	}

	void subtraction(number &a);

	void add(number &a);

	int cmp(number &a);


	number(string textNumber);
	number();
	number(number &a);

	number operator+=(number &a);
	number operator-=(number &a);
	number operator*=(number &a);
	number& operator=(number &a);


	friend int operator<(number &a, number &b);
	friend ostream& operator<<(ostream &out, number &a);
	friend istream& operator>>(istream &in, number &a);


	~number()
	{
		delete array;
	}
};

istream& operator>>(istream &in, number &a)
{
	delete a.array;
	a.array = new int[300];
	a.minus = 1;
	a.setZero();
	string aNumber;
	in>> aNumber;
	if(aNumber[0] == '-')
	{
		a.minus = -1;
		aNumber.erase(0, 1);
	}

	int length = aNumber.length();

	for(int i = 0; i < length; i++)
	{
		a.array[i] = aNumber[length - 1 - i] - '0';
	}

	a.size = length;

	return in;
}

int operator<(number &a, number &b)
{
	if(a.minus * b.minus == -1)
		return a.minus;
	else
	{
		int compare = a.cmp(b);


		return a.minus == 1 ? compare : -compare;
	}


}

ostream& operator<<(ostream &out, number &a)
{
	if(a.minus == -1)
		out<< '-';


	for(int i = a.size - 1; i >= 0; i--)
		out<< (char) (a.array[i] + '0');

	return out;
}



number::number()
{
	size = 1;
	array = new int[300];
	minus = 1;
	setZero();
}

number::number(string textNumber)
{


	minus = 1;
	array = new int[300];

	setZero();

	if(textNumber[0] == '-')
	{
		minus = -1;
		textNumber.erase(0, 1);
	}

	size = textNumber.length();
	for(int i = size - 1; i >= 0; i--)
		array[size - 1 - i] = textNumber[i] - '0';
}

number::number(number &a)
{


	size = a.size;
	minus = a.minus;
	array = new int[300];
	setZero();

	for(int i = 0; i < size; i++)
		array[i] = a.array[i];
}


number number::operator+=(number &a)
{

	if(minus == -1)
	{
		if(a.minus == -1)
		{
			add(a);
		}
		else
		{
			minus *= -1;
			(*this) -= a;
			minus *= -1;
		}
	}
	else
	{
		if(a.minus == -1)
		{
			a.minus *= -1;
			(*this) -= a;
			a.minus *= -1;
		}
		else
		{
			add(a);
		}
	}

	if(is_zero())
		minus = 1;


	return *this;
}

int number::cmp(number &a)
{
	if(size != a.size)
		return size < a.size ? -1 : 1;

	for(int i = size - 1; i >= 0; i--)
	{
		if(array[i] != a.array[i])
			return array[i] < a.array[i] ? -1 : 1;
	}
	return 0;
}

number number::operator-=(number &a)
{
	if(minus == -1)
	{
		if(a.minus == 1)
		{
			add(a);
			return *this;
		}
	}
	else
	{
		if(a.minus == -1)
		{
			add(a);
			return *this;
		}
	}

	if(cmp(a) < 0)
	{
		int tmpMinus = -minus;
		number first(*this);
		number sec(a);
		sec.subtraction(first);
		*this = sec;
		this -> minus = tmpMinus;
	}
	else
		subtraction(a);

	if(is_zero())
		minus = 1;

	return *this;
}

number number::operator*=(number &a)
{
	minus *= a.minus;

	if(a.size == 1 && a.array[0] == 0)
	{
		*this = a;
	}

	int *newArray = new int[300];

	for(int i = 0; i < 300; i++)
		newArray[i] = 0;

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < a.size; j++)
		{
			newArray[i + j] += array[i] * a.array[j];
		}
	}

	delete array;
	array = newArray;
	size += a.size - 1;
	multFormat();

	if(is_zero())
		minus = 1;

	return *this;
}

number& number::operator=(number &a)
{
	size = a.size;
	setZero();
	for(int i = 0; i < size; i++)
		array[i] = a.array[i];
	minus = a.minus;


	return *this;
}

void number::add(number &a)
{
	if(size < a.size)
		size = a.size;

	for(int i = 0; i < size; i++)
		array[i] += a.array[i];



	addFormat();
}

void number::subtraction(number &a)
{
	for(int i = 0; i < a.size; i++)
	{
		array[i] -= a.array[i];
	}

	subFormat();
}


/*
class number
{
	int *array;
	int size;
	int minus;

	void subFormat()
	{
		for(int i = 0; i < size; i++)
		{
			if(array[i] < 0)
			{
				if(array[i + 1] == 1 && i + 2 == size)
					size--;
				array[i] += 10;
				array[i + 1] -= 1;
			}
		}

		while(size > 1 && array[size - 1] == 0)
			size--;

	}

	void addFormat()
	{
		for(int i = 0; i < size; i++)
		{
			if(array[i] >= 10)
			{
				if(i + 1 == size)
					size++;
				array[i + 1] += array[i] / 10;
				array[i] %= 10;
			}
		}

		while(size > 1 && array[size - 1] == 0)
			size--;
	}

	void setZero()
	{
		for(int i = 0; i < 300; i++)
			array[i] = 0;
	}

public:

	number()
	{
		array = new int[300];
		minus = 1;
		setZero();
		string aNumber;
		cin>> aNumber;
		if(aNumber[0] == '-')
		{
			minus = -1;
			aNumber.erase(0, 1);
		}

		int length = aNumber.length();

		for(int i = 0; i < length; i++)
		{
			array[i] = aNumber[length - 1 - i] - '0';
		}

		size = length;
	}

	number(const number &aNumber)
	{
		size = aNumber.size;
		minus = aNumber.minus;
		array = new int[size];

		for(int i = 0; i < size; i++)
			array[i] = aNumber.array[i];
	}

	bool operator<(const number &aNumber)
	{
		if(size < aNumber.size)
			return true;
		if(size > aNumber.size)
			return false;

		int i;
		for(i = 0; i < size && array[i] == aNumber.array[i]; i++)
		{}

		if(i == size)
			return false;
		if(array[i] < aNumber.array[i])
			return true;

		return false;
	}

	bool operator==(const number &aNumber)
	{
		if(size != aNumber.size)
			return false;


		for(int i = 0; i < size; i++)
		{
			if(array[i] != aNumber.array[i])
				return false;
		}

		return true;
	}

	bool operator>(const number &aNumber)
	{
		if(size > aNumber.size)
			return true;
		if(size < aNumber.size)
			return false;

		int i;
		for(i = 0; i < size && array[i] == aNumber.array[i]; i++)
		{}

		if(i == size)
			return false;
		if(array[i] > aNumber.array[i])
			return true;

		return false;
	}

	void operator=(const number &aNumber)
	{
		delete array;
		size = aNumber.size;
		minus = aNumber.minus;
		array = new int[size];

		for(int i = 0; i < size; i++)
			array[i] = aNumber.array[i];
	}

	void add(const number &aNumber)
	{
		int length = size > aNumber.size ? size : aNumber.size;

		for(int i = 0; i < length; i++)
		{
			array[i] += aNumber.array[i];
		}


		size = length;
		addFormat();
	}

	void subtraction(const number &aNumber)
	{
		for(int i = 0; i < aNumber.size; i++)
		{
			array[i] -= aNumber.array[i];
		}

		subFormat();
	}

	void operator-=(number &aNumber);

	void operator+=(number &aNumber)
	{
		if(minus * aNumber.minus == 1)
		{
			add(aNumber);
		}
		else
		{
			aNumber.minus *= -1;
			*this -= aNumber;
		}
	}

	void operator*=(number &aNumber)
	{
		minus *= aNumber.minus;

		if(aNumber.size == 1 && aNumber.array[0] == 0)
		{
			*this = aNumber;
		}

		int *newArray = new int[300];

		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < aNumber.size; j++)
			{
				newArray[i + j] += array[i] * aNumber.array[j];
			}
		}

		delete array;
		array = newArray;
		size += aNumber.size;
		addFormat();
	}

	int cmp(number &aNumber)
	{
		if(minus == 1 && aNumber.minus == 1)
		{
			if(*this > aNumber)
				return 1;
			else if(*this == aNumber)
				return 0;
			else
				return -1;
		}
		if(minus == -1 && aNumber.minus == -1)
		{
			if(*this < aNumber)
				return 1;
			else if(*this == aNumber)
				return 0;
			else
				return -1;
		}
		if(minus == 1)
		{
			return 1;
		}
		else
			return -1;
	}

	~number()
	{
		delete [] array;
	}

	friend string to_string(const number &aNumber);
};

void number::operator-=(number &aNumber)
{
	if(minus == -1)
	{
		if(aNumber.minus == 1)
		{
			add(aNumber);
			return;
		}
	}
	else
	{
		if(aNumber.minus == -1)
		{
			add(aNumber);
			return;
		}
	}

	if((*this) < aNumber)
	{
		int tmpMinus = -minus;
		number tmp(*this);
		*this = aNumber;
		aNumber = tmp;
		minus = tmpMinus;
	}
	subtraction(aNumber);
}

string to_string(const number &aNumber)
{
	string ans = "";

	ans += aNumber.minus == -1 ? "-" : "";

	for(int i = aNumber.size - 1; i >= 0; i--)
		ans += aNumber.array[i] + '0';


	return ans;
}
*/
#endif //ALFA_SOLUTION_H
