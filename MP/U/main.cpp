#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Task
{
	unsigned int days;
	unsigned int deadline;
};

long getA(Task &t, long n)
{
	return t.deadline%n;
}

long getB(Task &t, long n)
{
	return (t.deadline/n)%n;
}

long getC(Task &t, long n)
{
	return t.deadline/(n*n);
}

void sort(Task *&array, long n, long (*get)(Task&, long))
{
	int *counter = new int[n]{0};
	for(int i = 0; i < n; i++)
	{
		counter[get(array[i], n)]++;
	}

	for(int i = 1; i < n; i++)
	{
		counter[i] += counter[i - 1];
	}


	Task *tmp = new Task[n];

	for(int i = n - 1; i >= 0; i--)
	{
		tmp[--counter[get(array[i], n)]] = array[i];
	}

	delete [] counter;
	delete [] array;
	array = tmp;
}

//TODO tak jak na wiki, mogę obliczyć pozycje na podstawie klucza, pętla nie jest po tablicy counter tylko po array

void print(Task *array, long n)
{
	for(int i = 0; i < n; i++)
	{
		cout<< array[i].deadline<< " ";
	}
	cout<< "\n";
}

pair<int, int> getFreeDays(Task *array, long n)
{
	long lastStart = array[n - 1].deadline - array[n - 1].days;
	int freeDays = 0;


	for(long i = n - 2; i >= 0; i--)
	{
		if(lastStart <= array[i].deadline)
		{
			lastStart = lastStart - array[i].days;
		}
		else
		{
			freeDays += lastStart - array[i].deadline;
			lastStart = array[i].deadline - array[i].days;
		}
	}

	pair<int, int> p(lastStart, freeDays);
	return p;
};

int main()
{
	ios_base::sync_with_stdio(false);

	int sets;
	cin>> sets;

	while(sets--)
	{
		long n;
		Task *array;

		cin>> n;
		array = new Task[n];

		for(int i = 0; i < n; i++)
		{
			cin>> array[i].days>> array[i].deadline;
		}

		sort(array, n, getA);
		sort(array, n, getB);
		sort(array, n, getC);
		print(array, n);

		pair<int, int> p = getFreeDays(array, n);

		if(p.first < 0)
			cout<< "ERROR\n";
		else
			cout<< p.first<< " "<< p.second<< "\n";

		delete [] array;
	}

	return 0;
}