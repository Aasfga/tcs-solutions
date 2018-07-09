#include <iostream>

using namespace std;

class Node;

class List;

class Iterator;

class Polynomial;


class Binomial
{
	friend string to_string(const Binomial &binomial);

	int degree;
	long long a;


public:

	Binomial(long long a, int degree)
	{
		this->degree = degree;
		this->a = a;
	}

	Binomial(const Binomial &binomial)
	{
		degree = binomial.degree;
		a = binomial.a;
	}

	Binomial operator+(const Binomial &binomial) const
	{
		Binomial ans(a + binomial.a, degree);
		return ans;
	}

	Binomial operator-(const Binomial &binomial) const
	{
		Binomial ans(a - binomial.a, degree);
		return ans;
	}

	Binomial operator*(int number) const
	{
		Binomial ans(a * number, degree);


		return ans;
	}

	Binomial operator*(const Binomial &binomial) const
	{
		Binomial ans(a * binomial.a, degree + binomial.degree);

		return ans;
	}

	Binomial &operator=(const Binomial &binomial)
	{
		a = binomial.a;
		degree = binomial.degree;

		return *this;
	}

	void operator+=(const Binomial &binomial)
	{
		if(degree == binomial.degree)
		{
			a += binomial.a;
		}
	}

	void operator-=(const Binomial &binomial)
	{
		if(degree == binomial.degree)
		{
			a -= binomial.a;
		}
	}

	void operator*=(int number)
	{
		a *= number;
	}

	void operator*=(const Binomial &binomial)
	{
		a *= binomial.a;
		degree += binomial.degree;
	}

	bool operator<(const Binomial &binomial) const
	{
		return degree < binomial.degree;
	}

	bool operator<=(const Binomial &binomial) const
	{
		return degree <= binomial.degree;
	}

	bool operator>(const Binomial &binomial) const
	{
		return degree > binomial.degree;
	}

	bool operator>=(const Binomial &binomial) const
	{
		return degree >= binomial.degree;
	}

	bool operator==(const Binomial &binomial) const
	{
		return degree == binomial.degree;
	}

	bool operator!=(const Binomial &binomial) const
	{
		return degree != binomial.degree;
	}

	void derivative()
	{
		a *= degree;
		degree--;
	}

	int getDegree() const
	{
		return degree;
	}

	long long int getA() const
	{
		return a;
	}

	void setDegree(int degree)
	{
		Binomial::degree = degree;
	}

	void setA(long long int a)
	{
		Binomial::a = a;
	}

	~Binomial()
	{}
};

string to_string(const Binomial &binomial)
{
	string ans = "";

	if(binomial.a == 0)
	{
		return ans;
	}

	if(binomial.a != 1 && binomial.a != -1)
	{
		ans += to_string(binomial.a);
	}

	if((binomial.a == 1 || binomial.a == -1) && binomial.degree == 0)
		ans += to_string(binomial.a);

	if(binomial.degree != 0)
	{
		if(binomial.a == -1)
			ans+= "-";
		ans += "x^" + to_string(binomial.degree);

	}

	return ans;
}

class Node
{
	friend class Iterator;

	friend class List;

	Node *next;
	Node *prev;
	Binomial binomial;

	Node(const Binomial &binomial, Node *prev, Node *next) : binomial(binomial)
	{
		this->next = next;
		this->prev = prev;
	}

};

class Iterator
{
	friend class List;

	List *list;
	Node *crr;

	Iterator(List *list);

public:

	Iterator(const Iterator &iter);

	void begin();

	void end();

	bool isEnd();

	bool isBegin();

	operator bool();

	void operator++();

	void operator++(int);

	void operator--();

	void operator--(int);

	void operator+(const Binomial &binomial);

	void operator-(const Binomial &binomial);


	void erase();

	Binomial &operator*();


};

class List
{
	friend class Iterator;

	Node *head;
	Node *tail;

public:

	List()
	{
		Binomial a(1, -1);
		Binomial b(1, 2000000000);
		head = new Node(a, NULL, NULL);
		tail = new Node(b, head, NULL);
		head->next = tail;
	}

	bool isEmpty()
	{
		return head->next == tail;
	}

	Iterator getIterator()
	{
		Iterator iter(this);

		return iter;
	}

	~List()
	{
		while(head != NULL)
		{
			Node *node = head->next;
			delete head;
			head = node;
		}
		//cout<<endl;
	}

};

class Polynomial
{
	List *binomials;

	void eraseZeros()
	{
		Iterator iter = binomials->getIterator();
		iter++;


		while(iter)
		{
			if((*iter).getA() == 0)
			{
				iter.erase();
			}
			else
			{
				iter++;
			}
		}

	}

public:

	Polynomial()
	{
		binomials = new List;
	}

	bool isEmpty()
	{
		return binomials->isEmpty();
	}

	void operator+=(const Binomial &binomial)
	{
		Iterator iter = binomials->getIterator();

		if(binomial.getA() == 0)
			return;

		while((*iter) < binomial)
		{
			iter++;
		}

		if((*iter) == binomial)
		{
			(*iter) += binomial;
		}
		else
		{
			iter - binomial;
		}

		if((*iter).getA() == 0)
		{
			iter.erase();
		}


	}

	void operator-=(const Binomial &binomial)
	{
		Iterator iter = binomials->getIterator();


		while((*iter) < binomial)
		{
			iter++;
		}

		if((*iter) == binomial)
		{
			(*iter) -= binomial;
		}
		else
		{
			Binomial b = binomial * -1;
			iter - b;
		}

		if((*iter).getA() == 0)
		{
			iter.erase();
		}

	}

	void operator+=(const Polynomial &poly)
	{
		Iterator first = binomials->getIterator();
		Iterator second = poly.binomials->getIterator();
		first++;
		second++;

		while(first && second)
		{
			if((*first) == (*second))
			{
				(*first) += (*second);
				first++;
				second++;
			}
			else if((*first) < (*second))
			{
				first++;
			}
			else
			{
				first - (*second);
				second++;
			}
		}

		while(second)
		{
			first - (*second);
			second++;
		}

		eraseZeros();
	}

	void operator-=(const Polynomial &poly)
	{
		Iterator first = binomials->getIterator();
		Iterator second = poly.binomials->getIterator();
		first++;
		second++;

		while(first && second)
		{
			if((*first) == (*second))
			{
				(*first) -= (*second);
				first++;
				second++;
			}
			else if((*first) < (*second))
			{
				first++;
			}
			else
			{
				second++;
			}
		}

		while(second)
		{
			Binomial b = (*second) * -1;
			first - b;
			second++;
		}


		eraseZeros();
	}

	void operator*=(int number)
	{
		Iterator iter = binomials->getIterator();
		iter++;

		while(iter)
		{
			(*iter) *= number;
			iter++;
		}

		eraseZeros();
	}

	long long max()
	{
		Iterator iter = binomials->getIterator();
		iter.end();
		iter--;

		return (*iter).getA();
	}

	long long min()
	{
		Iterator iter = binomials->getIterator();
		iter++;

		return (*iter).getA();
	}

	void derivative()
	{
		if(isEmpty())
		{
			return;
		}

		Iterator iter = binomials->getIterator();
		iter++;
		if((*iter).getDegree() == 0)
			iter.erase();

		while(iter)
		{
			(*iter).derivative();
			iter++;
		}
	}

	void printASC()
	{
		Iterator i = binomials->getIterator();
		i++;

		string ans = "";

		while(i)
		{
			if((*i).getA() <= 0)
			{
				ans += to_string((*i));
			}
			else
			{
				ans += "+" + to_string(*i);
			}

			i++;
		}


		if(ans[0] == '+')
		{
			ans.erase(0, 1);
		}

		cout << ans << "\n";
	}

	void printDSC()
	{
		Iterator i = binomials->getIterator();
		i.end();
		i--;

		string ans = "";

		while(i)
		{
			if((*i).getA() <= 0)
			{
				ans += to_string((*i));
			}
			else
			{
				ans += "+" + to_string(*i);
			}

			i--;
		}


		if(ans[0] == '+')
		{
			ans.erase(0, 1);
		}

		cout << ans << "\n";

	}

	void clean()
	{
		delete binomials;
		binomials = new List;
	}

	~Polynomial()
	{
		delete binomials;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);


	int sets;
	cin >> sets;

	while(sets--)
	{
		Polynomial polys[26];
		int tasks;
		cin >> tasks;

		while(tasks--)
		{
			string task;
			char id;
			cin >> task >> id;

			if(task == "PRINT_ASC")
			{
				if(polys[id - 'A'].isEmpty())
				{
					cout << "EMPTY\n";
				}
				else
				{
					polys[id - 'A'].printASC();
				}
			}
			else if(task == "PRINT_DESC")
			{
				if(polys[id - 'A'].isEmpty())
				{
					cout << "EMPTY\n";
				}
				else
				{
					polys[id - 'A'].printDSC();
				}

			}
			else if(task == "DERIVATIVE")
			{
				polys[id - 'A'].derivative();
			}
			else if(task == "ADD_MONO")
			{
				int a;
				int b;
				cin >> b >> a;

				Binomial binomial(a, b);

				polys[id - 'A'] += binomial;
				cout << "ADD OK\n";
			}
			else if(task == "ADD")
			{
				char second;
				cin >> second;

				polys[id - 'A'] += polys[second - 'A'];
				cout << "ADD OK\n";
			}
			else if(task == "MULTI_CONST")
			{
				int number;
				cin >> number;

				polys[id - 'A'] *= number;
				cout << "MULTI OK\n";
			}
			else if(task == "MULTI")
			{
			}
			else if(task == "MIN")
			{
				if(polys[id - 'A'].isEmpty())
				{
					cout << "ERROR\n";
				}
				else
				{
					cout << polys[id - 'A'].min() << "\n";
				}
			}
			else if(task == "MAX")
			{
				if(polys[id - 'A'].isEmpty())
				{
					cout << "ERROR\n";
				}
				else
				{
					cout << polys[id - 'A'].max() << "\n";
				}
			}
			else if(task == "CLEAN")
			{
				if(polys[id - 'A'].isEmpty())
				{
					cout << "EMPTY\n";
				}
				else
				{
					polys[id - 'A'].clean();
					cout<< "CLEAN OK\n";
				}
			}
		}
	}

	return 0;
}

Iterator::Iterator(List *list)
{
	this->list = list;
	crr = list->head;
}

Iterator::Iterator(const Iterator &iter)
{
	list = iter.list;
	crr = iter.crr;
}

void Iterator::begin()
{
	crr = list->head;
}

void Iterator::end()
{
	crr = list->tail;
}

bool Iterator::isEnd()
{
	return crr == list->tail;
}

bool Iterator::isBegin()
{
	return crr == list->head;
}

Iterator::operator bool()
{
	return !isEnd() && !isBegin();
}

void Iterator::operator++()
{
	if(*this)
	{
		return;
	}

	crr = crr->next;
}

void Iterator::operator++(int)
{
	if(isEnd())
	{
		return;
	}

	crr = crr->next;
}

void Iterator::operator--()
{
	if(isBegin())
	{
		return;
	}

	crr = crr->prev;
}

void Iterator::operator--(int)
{
	if(isBegin())
	{
		return;
	}

	crr = crr->prev;
}

Binomial &Iterator::operator*()
{
	return (crr->binomial);
}

void Iterator::operator+(const Binomial &binomial)
{
	if(isEnd())
	{
		return;
	}

	Node *newNode = new Node(binomial, crr, crr->next);
	crr->next->prev = newNode;
	crr->next = newNode;
}

void Iterator::operator-(const Binomial &binomial)
{
	if(isBegin())
	{
		return;
	}
	Node *newNode = new Node(binomial, crr->prev, crr);
	crr->prev->next = newNode;
	crr->prev = newNode;
}

void Iterator::erase()
{
	if(isBegin() || isEnd())
	{
		return;
	}


	crr->prev->next = crr->next;
	crr->next->prev = crr->prev;
	Node *node = crr->next;

	delete crr;

	crr = node;
}