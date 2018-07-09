//
// Created by Dominik Grybos on 12.03.2017.
//

#ifndef C_SOLUTION_H
#define C_SOLUTION_H

#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

template <class C>
class myPair
{
public:

	myPair()
	{}

	C first;
	C second;
};

template <class C>
class stack
{
	C *array;
	int maxSize;
	int last;

	void resize()
	{
		C *tmp = new C[maxSize * 2];

		for(int i = 0; i < maxSize; i++)
			tmp[i] = array[i];

		delete [] array;
		array = tmp;
		maxSize *= 2;
	}

public:

	stack(int n = 4)
	{
		maxSize = n;
		array = new C[maxSize];
		last = 0;
	}

	bool isEmpty()
	{
		return last == 0;
	}

	bool isFull()
	{
		return last == maxSize;
	}

	void push(C x)
	{
		if(isFull())
			resize();
		array[last++] = x;
	}

	void pop()
	{
		if(!isEmpty())
			last--;
	}

	C top()
	{
		return array[last - 1];
	}

	void clear()
	{
		last = 0;
	}

	int size()
	{
		return last;
	}

	~stack()
	{
		delete [] array;
	}
};

template <class C>
class stackMin
{
	stack<myPair<C> > aStack;

	myPair<C> createPair(C x)
	{
		myPair<C> pair;
		pair.first = x;

		if(aStack.isEmpty())
		{
			pair.second = x;
		}
		else if(aStack.top().second > x)
			pair.second = x;
		else
			pair.second = aStack.top().second;

		return pair;
	}

public:

	stackMin(int size = 4) : aStack(size)
	{}

	void push(C x)
	{
		aStack.push(createPair(x));
	}

	bool pop()
	{
		if(aStack.isEmpty())
			return false;
		aStack.pop();

		return true;
	}

	C top()
	{
		return aStack.top().first;
	}

	C min()
	{
		return aStack.top().second;
	}

	void clear()
	{
		aStack.clear();
	}

	bool empty()
	{
		return aStack.isEmpty();
	}

};

template <class C>
class queueMin
{
	stackMin<C> backStack;
	stackMin<C> frontStack;

	void moveElements()
	{
		while(!backStack.empty())
		{
			C x = backStack.top();
			backStack.pop();
			frontStack.push(x);
		}
	}

public:

	queueMin(int size = 4) : backStack(size), frontStack(size)
	{}

	void enqueue(C x)
	{
		backStack.push(x);
	}

	bool dequeue()
	{
		if(frontStack.empty())
			moveElements();

		return frontStack.pop();
	}

	C front()
	{
		if(frontStack.empty())
			moveElements();

		return frontStack.top();
	}

	C min()
	{
		if(frontStack.empty())
			return backStack.min();
		else if(backStack.empty())
			return frontStack.min();
		else
			return std::min(backStack.min(), frontStack.min());
	}

	void clear()
	{
		frontStack.clear();
		backStack.clear();
	}

	bool empty()
	{
		return frontStack.empty() && backStack.empty();
	}
};


template <class C>
void solveStack(stackMin<C> &S, int n)
{
	while(n--)
	{
		string order;
		cin>> order;

		if(order == "push")
		{
			C x;
			cin>> x;
			S.push(x);
		}
		else if(order == "pop")
		{
			if(S.empty())
				cout<< "ERROR\n";
			else
			{
				C x = S.top();
				S.pop();
				cout<< x<< "\n";
			}
		}
		else if(order == "top")
		{
			if(S.empty())
				cout<< "EMPTY\n";
			else
				cout<< S.top()<< "\n";
		}
		else if(order == "min")
		{
			if(S.empty())
				cout<< "EMPTY\n";
			else
				cout<< S.min()<< "\n";
		}
		else if(order == "clear")
		{
			S.clear();
		}
		else if(order == "empty")
		{
			if(S.empty())
				cout<< "YES\n";
			else
				cout<< "NO\n";
		}
	}
}

template <class C>
void solveQueue(queueMin<C> &S, int n)
{
	while(n--)
	{
		string order;
		cin>> order;

		if(order == "enqueue")
		{
			C x;
			cin>> x;
			S.enqueue(x);
		}
		else if(order == "dequeue")
		{
			if(S.empty())
				cout<< "ERROR\n";
			else
			{
				C x = S.front();
				S.dequeue();
				cout<< x<< "\n";
			}
		}
		else if(order == "front")
		{
			if(S.empty())
				cout<< "EMPTY\n";
			else
				cout<< S.front()<< "\n";
		}
		else if(order == "min")
		{
			if(S.empty())
				cout<< "EMPTY\n";
			else
				cout<< S.min()<< "\n";
		}
		else if(order == "clear")
		{
			S.clear();
		}
		else if(order == "empty")
		{
			if(S.empty())
				cout<< "YES\n";
			else
				cout<< "NO\n";
		}
	}
}

#endif //C_SOLUTION_H
