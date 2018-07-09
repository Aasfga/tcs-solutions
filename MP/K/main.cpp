#include <iostream>
#include <string.h>

int primes[15] = {61, 127, 241, 557, 1033, 2039, 4027, 8581, 16001, 32003, 64007, 128021, 256019, 512009, 1024021};

using namespace std;

class Field
{


public:

	char name[11];
	int score;

	Field()
	{
		name[0] = 0;
	}

	bool isFree()
	{
		return name[0] == 0;
	}

	bool isOccupied()
	{
		return score >= 0 && name[0] != 0;
	}

	bool isDeleted()
	{
		return score < 0 && name[0] != 0;
	}

	void setFree()
	{
		name[0] = 0;
	}

	void setOccupied()
	{
		if(score < 0)
			score *= -1;
	}

	void setDeleted()
	{
		if(score > 0)
			score *= -1;
	}

	bool operator!=(char string[])
	{
		return strcmp(name, string) != 0;
	}

	bool operator==(char string[])
	{
		return strcmp(name, string) == 0;
	}


};


class HashTable
{
	const unsigned long firstPrime = 4981760131;
	const unsigned long secondPrime = 6027502967;

	int size = 0;
	Field *fields;
	int nElems = 0;
	int deleted = 0;

	int hashFunction(char *name, unsigned long prime)
	{
		unsigned long value = 0;
		unsigned long p = 1;

		for(int i = 0; name[i] != 0; i++)
		{
			value += name[i] * p;
			p *= prime;
		}

		value %= primes[size];
		return (int) value;
	}

	void copy(Field fields[], int size)
	{
		for(int i = 0; i < size; i++)
		{
			if(fields[i].isOccupied())
				add(fields[i].name, fields[i].score);
		}
	}

	bool isAlmostFilled()
	{
		return 3 * primes[size] / 4 <= nElems;
	}

	bool isTooMuchPolluted()
	{
		return primes[size] / 4 <= deleted;
	}

	void resize(int howMuch)
	{
		size += howMuch;
		nElems = 0;
		deleted = 0;

		Field *oldFields = fields;
		fields = new Field[primes[size]];
		copy(oldFields, primes[size - howMuch]);
		delete [] oldFields;
	}

	void clean()
	{
		resize(0);
	}

	int findPosition(char *name)
	{
		int position = hashFunction(name, firstPrime);
		int move = hashFunction(name, secondPrime);
		move = move == 0 ? 1 : move;

		int i = position;
		while(!fields[i].isFree() && fields[i] != name)
		{
			i = (i + move) % primes[size];
		}

		return i;
	}

	int findFirstDelete(char *name)
	{
		int position = hashFunction(name, firstPrime);
		int move = hashFunction(name, secondPrime);
		move = move == 0 ? 1 : move;
		int i = position;
		while(fields[i].isOccupied() && fields[i] != name)
			i = (i + move) % primes[size];



		return fields[i].isDeleted() ? i : -1;
	}

public:

	void print()
	{
		for(int i = 0; i < primes[size]; i++)
		{
			cout << i << ": " << fields[i].name << " "<< fields[i].score << "\n";
		}
	}

	HashTable()
	{
		fields = new Field[primes[size]];
	}

	void add(char *name, int score)
	{
		if(isAlmostFilled())
		{
			resize(1);
		}

		int i = findPosition(name);
		int del = findFirstDelete(name);

		if(fields[i].isOccupied() && fields[i] == name)
		{
			if(score > fields[i].score)
			{
				fields[i].score = score;
			}
		}
		else if(fields[i].isDeleted() && fields[i] == name)
		{
			fields[i].score = score;
			fields[i].setOccupied();
			nElems++;
			deleted--;
		}
		else
		{
			if(del != -1)
			{
				i = del;
				deleted--;
			}
			nElems++;
			fields[i].setOccupied();
			strcpy(fields[i].name, name);
			fields[i].score = score;
		}
	}

	pair<string, int> find(char *name)
	{
		int i = findPosition(name);

		pair<string, int> p("ERROR", 1);

		if(!fields[i].isOccupied())
		{
			return p;
		}
		else
		{
			p.first = fields[i].name;
			p.second = fields[i].score;

			return p;
		}
	};

	pair<string, int> erase(char *name)
	{
		if(isTooMuchPolluted())
		{
			clean();
		}


		int i = findPosition(name);

		pair<string, int> p("ERROR", -1);

		if(!fields[i].isOccupied())
		{
			return p;
		}
		else
		{
			deleted++;
			nElems--;

			p.first = fields[i].name;
			p.second = fields[i].score;
			fields[i].setDeleted();

			return p;
		}
	};

	~HashTable()
	{
		delete [] fields;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);

	int sets;
	cin >> sets;

	while(sets--)
	{
		int n;
		cin >> n;
		HashTable *table = new HashTable;
		string task;
		char name[11];
		int score;

		while(n--)
		{
			cin >> task >> name;

			if(task == "INSERT")
			{
				cin >> score;
				table -> add(name, score);
			}
			else if(task == "FIND")
			{
				pair<string, int> p = table -> find(name);
				if(p.first == "ERROR")
				{
					cout << "ERROR" << "\n";
				}
				else
				{
					cout << p.second << "\n";
				}
			}
			else if(task == "DELETE")
			{
				pair<string, int> p = table -> erase(name);
				if(p.first == "ERROR")
				{
					cout << "ERROR" << "\n";
				}
				else
				{
					cout << p.first << " " << p.second << "\n";
				}
			}
		}

		delete table;
	}


	return 0;
}