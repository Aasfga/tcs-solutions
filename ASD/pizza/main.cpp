#include <iostream>

using namespace std;

int main()
{
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		int n;
		cin>> n;
		int *olives = new int[n];
		for(int i = 0; i < n; i++)
			cin>> olives[i];
		int **array = new int*[n + 1];
		int **sum_array = new int*[n+1];
		for(int i = 0; i <= n; i++)
		{
			array[i] = new int[n]{0};
			sum_array[i] = new int[n]{0};

		}

		for(int d = 1; d <= n; d++)
		{
			for(int i = 0; i < n; i++)
			{
				sum_array[d][i] = sum_array[d - 1][i] + olives[(i + d - 1) % n];
				array[d][i] = sum_array[d][i];
				array[d][i] -= min(array[d - 1][i], array[d - 1][(i+1)%n]);
			}
		}

		int sum = 0;
		for(int i = 0; i < n; i++)
			sum += olives[i];
		int max_ = 0;
		for(int i = 0; i < n; i++)
			max_ = max(array[n][i], max_);
		cout<< max_<< " "<< sum - max_<< "\n";


		for(int i = 0; i <= n; i++)
		{
			delete [] array[i];
			delete [] sum_array[i];

		}
		delete [] sum_array;
		delete [] array;
		delete [] olives;
	}
	return 0;
}