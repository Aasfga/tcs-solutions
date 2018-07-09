#include <iostream>

using namespace std;


bool** allocate_array(int n)
{
	bool **array = new bool*[n];

	for(int i = 0; i < n; i++)
	{
		array[i] = new bool[n];
	}

	return array;
}

void read_array(bool **array, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			char _char;
			cin>> _char;
			array[i][j] = _char == 'o';
		}
	}
}

void delete_array(bool **array, int n)
{
	for(int i = 0; i < n; i++)
		delete [] array[i];

	delete [] array;
}

int count_bits(unsigned int x)
{
	return __builtin_popcount(x);
}


int main()
{
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		int n;
		cin>> n;
		bool **holes = allocate_array(n);
		long long *array = new long long[1 << (n + 1)]{0};
		read_array(holes, n);


		array[0] = 1;
		for(unsigned int i = 1; i < (1 << n); i++)
		{
			for(int j = 0; (1 << j) <= i; j++)
			{
				if((i & (1 << j)) == 0)
					continue;
				if(holes[count_bits(i) - 1][j])
					continue;
				array[i] += array[i - (1<<j)];
			}
		}


		cout<< array[(1 << n) - 1]<< "\n";

		delete [] array;
		delete_array(holes, n);
	}
	return 0;
}