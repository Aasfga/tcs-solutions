#include <iostream>
#include <algorithm>

using namespace std;


int binSearch(int *array, int n, int x)
{
	int a = 0;
	int b = n;

	while(a != b)
	{
		int s = (a + b + 1)/2;
		int v = array[s];
		if(v < x)
			a = s;
		else
			b = s - 1;
	}


	return a;
}

int main()
{
	int sets;
	cin>> sets;

	while(sets--)
	{
		int n;
		cin>> n;

		int *data = new int[n];
		for(int i = 0; i < n; i++)
			cin>> data[i];
		int *array = new int[n + 1]{-1};
		fill_n(array + 1, n, INT32_MAX);

		for(int i = 0; i < n; i++)
		{
			int x = binSearch(array, n, data[i]);
			array[x + 1] = data[i];
		}

		int ans = -1;
		for(int i = n; i > 0; i--)
		{
			if(array[i] != INT32_MAX)
			{
				ans = i;
				break;
			}
		}

		cout<< n - ans<< "\n";

		delete [] array;
		delete [] data;
	}
	return 0;
}