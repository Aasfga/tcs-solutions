#include <iostream>
#include <algorithm>

using namespace std;

void printArray(int *array, int size)
{
	for(int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	cout << "\n";
}


void sortFives(int *array, int begin, int jump)
{
	int tmp[5];

	for(int i = 0; i < 5; i++)
	{
		tmp[i] = array[begin + i * jump];
	}
	sort(tmp, tmp + 5);

	for(int i = 0; i < 5; i++)
	{
		array[begin + i * jump] = tmp[i];
	}

}

int* magicFives(int *array, int size)
{
	if(size < 20)
	{
		sort(array, array + size);
		return array + size/2;
	}

	size = size / 5 * 5;

	for(int i = 0; i < size / 5; i++)
	{
		sortFives(array, i, size / 5);
	}

	return magicFives(array + 2 * size / 5, size / 5);
}

pair<int, int> partition(int *array, int size)
{

	int *x = magicFives(array, size);
	int y = *x;
	swap(array[0], *x);
	int i = 0;
	int j = size - 1;
	int k = 0;

	while(j >= k)
	{
		if(array[i] == y)
		{
			swap(array[i], array[k++]);
		}
		else if(array[i] < y)
		{
			i++;
		}
		else if(array[i] > y)
		{
			swap(array[i], array[j--]);
		}
	}
	if(array[i] > array[k - 1])
	{
		swap(array[i], array[--k]);
	}
	if(array[i] == y)
	{
		i--;
	}
	if(array[k] == y)
	{
		k++;
	}
	pair<int, int> p(i, k);
	return p;
}

int getKth(int *array, int k, int size)
{
	pair<int, int> p = partition(array, size);
//	printArray(array, size);
	if(k > p.first && k < p.second)
		return array[p.first + 1];
	else if(k <= p.first)
		return getKth(array, k, p.first + 1);
	else
		return getKth(array + p.second , k - p.second, size - p.second);
}

int main()
{
	ios_base::sync_with_stdio(false);

	int z;
	cin>> z;

	while(z--)
	{
		int size;
		int k;
		int *array;
		cin>> size>> k;
		k--;

		array = new int[size];

		for(int i = 0; i < size; i++)
			cin>> array[i];
		cout<< getKth(array, k, size)<< "\n";
		delete[] array;
	}

	return 0;
}