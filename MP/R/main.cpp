#include <iostream>

using namespace std;

long merge(int *array, int begin, int mid, int end, int d)
{
	int *copyOfArray = new int[end - begin + 1];

	long inv = 0;
	int i = begin;
	int j = mid + 1;
	int k = 0;
	int p = begin;
	while(i <= mid && j <= end)
	{
		if(array[i] < array[j])
		{
			while(p <= mid && array[p] + d >= array[j])
				p++;
			inv += mid - p + 1;

			copyOfArray[k++] = array[j++];
		}
		else
		{
			copyOfArray[k++] = array[i++];
		}
	}

	while(i <= mid)
	{
		copyOfArray[k++] = array[i++];
	}
	while(j <= end)
	{
		copyOfArray[k++] = array[j++];
	}

	for(int i = 0; i < end - begin + 1; i++)
	{
		array[begin + i] = copyOfArray[i];
	}

	delete [] copyOfArray;

	return inv;
}

long mergeSort(int *array, int begin, int end, int d)
{
	if(begin >= end)
		return 0;

	long inv = 0;

	int mid = (begin + end)/2;
	inv += mergeSort(array, begin, mid, d);
	inv += mergeSort(array, mid + 1, end, d);
	inv += merge(array, begin, mid, end, d);


	return inv;
}

void readArray(int *array, int size)
{
	for(int i = 0; i < size; i++)
		cin>> array[i];
}

void printArray(int *array, int size)
{
	for(int i = 0; i < size; i++)
		cout<< array[i]<< " ";
	cout<< "\n";
}

int main()
{

	ios_base::sync_with_stdio(false);

	int sets;
	cin>> sets;

	while(sets--)
	{
		int *array;
		int size;
		int d;
		cin>> size;
		cin>> d;
		array = new int[size];
		readArray(array, size);
		long inv = mergeSort(array, 0, size - 1, d);
		//printArray(array, size);
		cout<< inv<< "\n";

		delete [] array;
	}

	return 0;
}