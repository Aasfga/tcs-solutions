#include <iostream>
#include <string>

using namespace std;

string to_string(int *array, int size)
{
	string ans;

	for(int i = 0; i < size; i++)
		ans += to_string(array[i]) + " ";

	return ans;
}

int binSearch(int *array, int left, int right, int x, int sub)
{

	while(left != right)
	{
		int s = (left + right + 1)/2;

		if(array[s] - sub <= x)
			left = s;
		else
			right = s - 1;
	}


	return left;
}

int* createSumArray(int *array, int size)
{
	int *sum = new int[size];
	int crrSum = 0;

	for(int i = 0; i < size; i++)
	{
		sum[i] = crrSum + array[i];
		crrSum += array[i];
	}

	return sum;
}

/*
void sub(int *array, int size, int x)
{
	for(int i = 0; i < size; i++)
		array[i] -= x;
}
*/
int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int *array, int size)
{
	int min = array[0];
	for(int i = 1; i < size; i++)
	{
		if(array[i] < min)
			min = array[i];
	}

	return min;
}

int max(int *array, int size)
{
	int max = array[0];
	for(int i = 1; i < size; i++)
	{
		if(array[i] > max)
			max = array[i];
	}

	return max;
}

int squareRoot(int x)
{
	int i;
	for(i = 1; i*i < x; i++);

	return i;
}

bool setSoldiers(int *sum, int size, int a)
{
	int left = 0;
	int sub = 0;
	for(int i = 1; i <= a && left != size; i++)
	{
		left = binSearch(sum, left, size - 1, a, sub);
		sub += sum[left] - sub;
	}


	return size == left + 1;
}

int minSquare(int *array, int size)
{
	int *sum = createSumArray(array, size);
	int left = max(squareRoot(sum[size - 1]), max(array, size));
	int right = max(size, max(array, size));

	while(left != right)
	{
		int s = (left + right)/2;

		if(setSoldiers(sum, size, s))
			right = s;
		else
			left = s + 1;
	}

	delete [] sum;
	return left;
}

int main()
{
	ios_base::sync_with_stdio(false);


	int tests;
	cin>> tests;

	while(tests--)
	{
		int size;
		cin>> size;
		int *array = new int[size];

		for(int i = 0; i < size; i++)
			cin>> array[i];

		cout<< minSquare(array, size)<< "\n";

		delete [] array;

	}
	
	return 0;
}