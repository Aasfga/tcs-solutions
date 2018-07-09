#include <iostream>

using namespace std;

class Person
{
	int age;
	string name;

public:

	Person(string name, int age) : age(age), name(name)
	{}

	Person(const Person &person)
	{
		age = person.age;
		name = person.name;
	}

	bool operator==(const Person &person) const
	{
		return person.age == age && person.name == name;
	}

	bool operator!=(const Person &person) const
	{
		return !(*this == person);
	}

	int getAge() const
	{
		return age;
	}

	const string &getName() const
	{
		return name;
	}

	void print()
	{
		cout<< name<< " "<< age<< "\n";
	}
};

class Node
{
	friend class List;

	Person *person;
	Node *next;

	Node(const Person &person, Node *next = NULL)
	{
		this->person = new Person(person);
		this->next = next;
	}

	Node(const Node &node)
	{
		person = new Person(*node.person);
		next = node.next;
	}

	~Node()
	{
		delete person;
	}
};

class List
{
	Node *head;
	Node *last;

public:

	List()
	{
		Person p("HEAD", -1);
		head = new Node(p);
		last = head;
	}

	void addEnd(const Person &person)
	{
		Node *node = new Node(person);
		last->next = node;
		last = node;
	}

	void addBegin(const Person &person)
	{

		Node *node = new Node(person, head->next);
		head->next = node;
		if(head == last)
		{
			last = node;
		}
	}

	int popEnd()
	{
		if(head == last)
		{
			return 1;
		}
		Node *node = head;

		while(node->next != last)
		{
			node = node->next;
		}

		delete last;
		last = node;
		node->next = NULL;

		return 0;
	}

	int popFront()
	{
		if(head == last)
		{
			return 1;
		}
		if(head->next == last)
		{
			delete last;
			last = head;
			head->next = NULL;
		}
		else
		{
			Node *node = head->next->next;
			delete head->next;
			head->next = node;
		}

		return 0;
	}

	bool isEmpty()
	{
		return head == last;
	}

	int insert(const Person &person, int n)
	{
		Node *node = head;

		while(--n > 0 && node != NULL)
		{
			node = node -> next;
		}

		if(node == NULL)
			return 1;

		Node *newNode = new Node(person, node -> next);
		node -> next = newNode;

		last = node == last ? newNode : last;

		return 0;
	}

	int erase(const Person &person)
	{
		Node *node = head;


		while( node -> next != NULL && *(node -> next -> person) != person)
		{
			node = node->next;
		}

		if(node -> next == NULL)
			return 1;

		Node *old = node -> next;

		if(old == last)
			last = node;

		node -> next = node -> next -> next;

		delete old;

		return 0;
	}


	int reverse()
	{
		if(head == last)
			return 1;

		Node helper(*head);
		head -> next = head -> next -> next;
		helper.next -> next = NULL;
		last = helper.next;

		while(head -> next != NULL)
		{
			Node *tmp = head -> next -> next;
			head -> next -> next = helper.next;
			helper.next = head -> next;
			head -> next = tmp;
		}

		head -> next = helper.next;

		return 0;
	}

	int print()
	{
		if(head == last)
			return 1;

		Node *node = head -> next;

		while(node != NULL)
		{
			node -> person -> print();
			node = node -> next;
		}

		return 0;
	}

	~List()
	{
		Node *next = head->next;

		while(next != NULL)
		{
			delete head;
			head = next;
			next = head->next;
		}

		delete head;
	}
};


int main()
{
	ios_base::sync_with_stdio(false);

	int sets;
	cin>> sets;

	while(sets--)
	{
		int tasks;
		cin>> tasks;

		List *list = new List;

		while(tasks--)
		{
			string task;

			cin>> task;

			string name;
			int age;
			int n;

			if(task == "NEW")
			{
				cin>> name >> age;
				Person p(name, age);
				list -> addEnd(p);
			}
			else if(task == "VIP")
			{
				cin>> name >> age;
				Person p(name, age);
				list -> addBegin(p);
			}
			else if(task == "INSERT")
			{
				cin>> name >> age >> n;
				Person p(name, age);
				if(list -> insert(p, n))
					cout<< "ERROR\n";
			}
			else if(task == "BUYING")
			{
				if(list -> popFront())
					cout<< "ERROR\n";
			}
			else if(task == "RESIGNATION")
			{
				if(list -> popEnd())
					cout<< "ERROR\n";
			}
			else if(task == "DELETE")
			{
				cin>> name >> age;
				Person p(name, age);
				if(list -> erase(p))
					cout<< "ERROR\n";
			}
			else if(task == "PRINT")
			{
				if(list-> print())
					cout<< "EMPTY\n";
			}
			else if(task == "REVERSE")
			{
				if(list -> reverse())
					cout<< "EMPTY\n";
			}
			else if(task == "CLEAN")
			{
				delete list;
				list = new List;
			}

			int a = 0;


		}

		delete list;
	}






	return 0;
}