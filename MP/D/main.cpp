#include <iostream>

using namespace std;

template<class T>
class node
{
public:
	T info;
	node<T> *links[2];

	node(T info, node<T> *prev, node<T> *next)
	{
		this->info = info;
		links[0] = prev;
		links[1] = next;
	}

	node<T> *next(node<T> *last)
	{
		return links[last == links[0]];
	}

	node<T> *prev(node<T> *next)
	{
		return links[next == links[0]];
	}

	void setNext(node<T> *last, node<T> *newNode)
	{
		links[last == links[0]] = newNode;
	}

	void setPrev(node<T> *next, node<T> *newNode)
	{
		links[next == links[0]] = newNode;
	}

};


template<class T>
class doubleList
{
public:

	string name;
	node<T> *head;
	node<T> *tail;

	doubleList(string name)
	{
		this->name = name;
		T t;
		head = new node<T>(t, NULL, NULL);
		tail = new node<T>(t, head, NULL);
		head->links[1] = tail;
	}

	void addFirst(T &x)
	{
		node<T> *newNode = new node<T>(x, head, head->next(NULL));
		insertFirst(newNode);
	}

	void addLast(T &x)
	{
		node<T> *newNode = new node<T>(x, tail->prev(NULL), tail);
		insertLast(newNode);
	}

	void addFirst(node<T> *node)
	{
		node->links[0] = head;
		node->links[1] = head->next(NULL);
		insertFirst(node);
	}

	void addLast(node<T> *node)
	{
		node->links[0] = tail->prev(NULL), tail;
		node->links[1] = tail;
		insertLast(node);
	}

	void insertFirst(node<T> *newNode)
	{
		head->next(NULL)->setPrev(head->next(NULL)->next(head), newNode);
		head->setNext(NULL, newNode);
	}

	void insertLast(node<T> *newNode)
	{
		tail->prev(NULL)->setNext(tail->prev(NULL)->prev(tail), newNode);
		tail->setPrev(NULL, newNode);
	}

	void printBegin()
	{
		node<T> *i = head->next(NULL);
		node<T> *last = head;

		while(i->next(last) != tail)
		{
			cout << i->info << "<-";
			node<T> *tmp = i;
			i = i->next(last);
			last = tmp;
		}
		cout << i->info << "\n";
	}

	void printEnd()
	{
		node<T> *i = tail->prev(NULL);
		node<T> *last = tail;

		while(i != head)
		{
			cout << i->info << " ";
			node<T> *tmp = i;
			i = i->prev(last);
			last = tmp;
		}

		cout << "\n";
	}

	node<T> *detachFront()
	{
		if(isEmpty())
		{
			return NULL;
		}

		node<T> *toDelete = head->next(NULL);
		node<T> *newFirst = head->next(NULL)->next(head);
		newFirst->setPrev(newFirst->next(toDelete), head);
		head->setNext(NULL, newFirst);

		return toDelete;
	}

	node<T> *detachLast()
	{
		if(isEmpty())
		{
			return NULL;
		}

		node<T> *toDelete = tail->prev(NULL);
		node<T> *newFirst = tail->prev(NULL)->prev(tail);
		newFirst->setNext(newFirst->prev(toDelete), tail);
		tail->setPrev(NULL, newFirst);

		return toDelete;
	}

	bool isEmpty()
	{
		return head->next(NULL) == tail;
	}

	void clean()
	{
		while(!isEmpty())
		{
			delete detachFront();
		}
	}

	void reverse()
	{
		swap(head, tail);
	}

	void unionn(doubleList<T> &list)
	{
		tail->prev(NULL)->setNext(tail->prev(NULL)->prev(tail), list.head->next(NULL));
		list.head->next(NULL)->setPrev(list.head->next(NULL)->next(list.head), tail->prev(NULL));
		delete list.head;
		delete tail;
		tail = list.tail;
		T t;
		list.head = new node<T>(t, NULL, NULL);
		list.tail = new node<T>(t, list.head, NULL);
		list.head->links[1] = list.tail;

	}


	~doubleList()
	{
		clean();

		delete head;
		delete tail;
	}
};

void setAllToNull(doubleList<string> **list)
{
	for(int i = 0; i < 20; i++)
	{
		list[i] = NULL;
	}
}

doubleList<string> **findNull(doubleList<string> **list)
{
	for(int i = 0; i < 20; i++)
	{
		if(list[i] == NULL)
		{
			return &list[i];
		}
	}

	return NULL;
}

doubleList<string> **findTrain(doubleList<string> **list, string name)
{
	for(int i = 0; i < 20; i++)
	{
		if(list[i] != NULL && list[i]->name == name)
		{
			return &list[i];
		}
	}

	return NULL;
}

void deleteTrain(doubleList<string> **train)
{
	if(*train != NULL)
	{
		delete *train;
	}

	*train = NULL;
}

void deleteAllTrains(doubleList<string> **trains)
{
	for(int i = 0; i < 20; i++)
	{
		deleteTrain(&trains[i]);
	}

	delete trains;
}

void newTrain(doubleList<string> **trains, string name, string wagon)
{
	doubleList<string> **train = findNull(trains);
	*train = new doubleList<string>(name);
	(*train)->addFirst(wagon);
}

void back(doubleList<string> **trains, string name, string wagon)
{
	doubleList<string> **train = findTrain(trains, name);
	(*train)->addLast(wagon);
}

void front(doubleList<string> **trains, string name, string wagon)
{
	doubleList<string> **train = findTrain(trains, name);
	(*train)->addFirst(wagon);
}

void print(doubleList<string> **trains, string name, string wagon)
{
	doubleList<string> **train = findTrain(trains, name);
	(*train)->printBegin();
}

void reverse(doubleList<string> **trains, string name)
{
	doubleList<string> **train = findTrain(trains, name);
	(*train)->reverse();
}

void unionn(doubleList<string> **trains, string first, string second)
{
	doubleList<string> **firstTrain = findTrain(trains, first);
	doubleList<string> **secondTrain = findTrain(trains, second);
	(*firstTrain)->unionn(**secondTrain);
	deleteTrain(secondTrain);
}

void delFront(doubleList<string> **trains, string brandNewTrain, string oldTrain)
{
	doubleList<string> **secondTrain = findTrain(trains, oldTrain);
	node<string> *node = (*secondTrain)->detachFront();
	string wagon = node->info;
	delete node;

	if((*secondTrain)->isEmpty())
	{
		deleteTrain(secondTrain);
	}

	newTrain(trains, brandNewTrain, wagon);
}

void delBack(doubleList<string> **trains, string brandNewTrain, string oldTrain)
{
	doubleList<string> **secondTrain = findTrain(trains, oldTrain);
	node<string> *node = (*secondTrain)->detachLast();
	string wagon = node->info;
	delete node;

	if((*secondTrain)->isEmpty())
	{
		deleteTrain(secondTrain);
	}

	newTrain(trains, brandNewTrain, wagon);
}

void print(doubleList<string> **trains, string name)
{
	doubleList<string> **train = findTrain(trains, name);
	cout << "\"" << (*train)->name << "\":\n";
	(*train)->printBegin();
}

void printAll(doubleList<string> **trains)
{
	for(int i = 0; i < 20; i++)
	{
		if(trains[i] != NULL)
		{
			cout << "\"" << trains[i]->name << "\":\n";
			trains[i]->printBegin();
		}
	}
}

void newParty(int n)
{
	doubleList<string> **trains = new doubleList<string> *[20];
	setAllToNull(trains);
	for(int i = 0; i < n; i++)
	{
		string task, first, second;
		cin >> task;

		if(task == "NEW")
		{
			cin >> first >> second;
			newTrain(trains, first, second);
		}
		else if(task == "BACK")
		{
			cin >> first >> second;
			back(trains, first, second);
		}
		else if(task == "FRONT")
		{
			cin >> first >> second;
			front(trains, first, second);
		}
		else if(task == "PRINT")
		{
			cin >> first;
			print(trains, first);
		}
		else if(task == "REVERSE")
		{
			cin >> first;
			reverse(trains, first);
		}
		else if(task == "UNION")
		{
			cin >> first >> second;
			unionn(trains, first, second);
		}
		else if(task == "DELFRONT")
		{
			cin >> first >> second;

			delFront(trains, first, second);
		}
		else if(task == "DELBACK")
		{
			cin >> first>> second;
			delBack(trains, second, first);
		}
	}

	deleteAllTrains(trains);
}

int main()
{
	int z;
	cin>> z;

	while(z--)
	{
		int n;
		cin>> n;
		newParty(n);
	}

	return 0;
}