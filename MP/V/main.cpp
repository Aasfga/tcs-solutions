#include <iostream>

using namespace std;


class Mandate
{
	unsigned long p;
	unsigned long q = 1;
	int id;

public:

	Mandate()
	{}

	Mandate(int p, int id) : p(p), id(id)
	{}

	int getID()
	{
		return id;
	}

	void increaseQ()
	{
		q++;
	}

	bool operator<(Mandate &m)
	{
		if((long) (p * m.q) < (long) (m.p * q))
			return true;
		else if((long) (p * m.q) > (long) (m.p * q))
			return false;
		else
		{
			if(p < m.p)
				return true;
			else if(p > m.p)
				return false;
			else
			{
				return id > m.id;
			}
		}
	}

	bool operator>(Mandate &m)
	{
		if((long) (p * m.q) > (long) (m.p * q))
			return true;
		else if((long) (p * m.q) < (long) (m.p * q))
			return false;
		else
		{
			if(p > m.p)
				return true;
			else if(p < m.p)
				return false;
			else
			{
				return id < m.id;
			}
		}

	}

	void print()
	{
		cout << p << " " << q << "\n";
	}
};


class Heap
{
	int size;
	Mandate *mandates;

	void insert(int i)
	{
		if(i >= size)
		{
			return;
		}

		int a = 2 * i;
		int b = 2 * i + 1;

		if(a >= size)
		{
			return;
		}
		if(b >= size && mandates[a] > mandates[i])
		{
			swap(mandates[a], mandates[i]);
		}
		else if(b < size)
		{
			int max = mandates[a] > mandates[b] ? a : b;

			if(mandates[max] > mandates[i])
			{
				swap(mandates[max], mandates[i]);
				insert(max);
			}
		}
	}

	void construct(int i)
	{
		if(i >= size)
		{
			return;
		}

		construct(2 * i);
		construct(2 * i + 1);
		insert(i);
	}

public:


	Heap(int n) : size(n + 1), mandates(new Mandate[size])
	{
		for(int i = 1; i < size; i++)
		{
			int p;
			cin >> p;
			mandates[i] = Mandate(p, i);
		}
		construct(1);
	}

	void print(int i = 1, int t = 0)
	{
		if(i >= size)
			return;

		for(int j = 0; j < t; j++)
			cout << "\t";
		mandates[i].print();
		print(2 * i, t + 1);
		print(2 * i + 1, t + 1);
	}

	Mandate getMax()
	{
		return mandates[1];
	}

	void add(Mandate m)
	{
		mandates[size - 1] = m;

		int k = size - 1;

		while(k > 1)
		{
			if(mandates[k] > mandates[k / 2])
			{
				swap(mandates[k], mandates[k / 2]);
				k /= 2;
			}
			else
				break;
		}
	}

	void remove()
	{
		swap(mandates[1], mandates[size - 1]);
		mandates[size - 1] = Mandate(0, 2000);
		insert(1);
	}

	~Heap()
	{
		delete[] mandates;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);

	int sets;
	cin >> sets;

	while(sets--)
	{
		int size;
		int mandates;
		int *count;
		cin >> size >> mandates;
		count = new int[size + 1]{0};
		Heap heap(size);

		while(mandates--)
		{
			Mandate m = heap.getMax();
			m.increaseQ();
			heap.remove();
			heap.add(m);
			count[m.getID()]++;
		}

		for(int i = 1; i < size + 1; i++)
			cout << count[i] << " ";
		cout << "\n";
		delete[] count;
	}
}
