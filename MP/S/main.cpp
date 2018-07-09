#include <iostream>


using namespace std;


pair<int, int> partition(int *array, int begin, int end)
{
	int k = rand() % (end - begin) + begin;
	int x = array[k];
	swap(array[k], array[begin]);
	int i = begin;
	int j = end;
	k = begin;

	while(j >= k)
	{
		if(array[i] == x)
		{
			swap(array[i], array[k++]);
		}
		else if(array[i] < x)
		{
			i++;
		}
		else if(array[i] > x)
		{
			swap(array[i], array[j--]);
		}
	}
	if(array[i] > array[k - 1])
	{
		swap(array[i], array[--k]);
	}
	if(array[i] == x)
	{
		i--;
	}
	if(array[k] == x)
	{
		k++;
	}
	pair<int, int> p(i, k);
	return p;
};

void quickSort(int *array, int begin, int end)
{

	while(begin < end)
	{
		pair<int, int> p = partition(array, begin, end);
		if(p.first - begin < end - p.second)
		{
			quickSort(array, begin, p.first);
			begin = p.second;
		}
		else
		{
			quickSort(array, p.second, end);
			end = p.first;
		}
	}
}

int count(int *array, int size, int d)
{
	int p = 0;
	int c = 1;
	for(int i = 1; i < size; i++)
	{
		while(p < i && array[p] + d < array[i])
			p++;
		c *= i - p + 1;
		c %= 10000;
	}

	return c;
}

void print(int x)
{
	cout<< x/1000<< x/100 %10 << x/10 % 10<< x%10<<"\n";
}

int main()
{

	int sets;
	cin>> sets;

	while(sets--)
	{
		int size;
		int d;
		int *array;

		cin>> size>> d;
		array = new int[size];
		for(int i = 0; i < size; i++)
			cin>> array[i];
		quickSort(array, 0, size - 1);

//		for(int i = 0; i < size; i++)
//			cout<< array[i]<< " ";
//		cout<< "\n";

		print(count(array, size, d));


		delete [] array;
	}
	return 0;
}