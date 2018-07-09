#include <iostream>

using namespace std;

int main()
{
	int sets;
	cin>>sets;


	while(sets--)
	{
		int n;
		int b;
		cin>>n>>b;
		auto value = new int[n + 1]{0};
		auto space = new int[n + 1]{0};
		auto array = new long long *[n + 1];

		for(int i = 0; i <= n; i++)
			array[i] = new long long[b + 1]{0};
		for(int i = 0; i < n; i++)
			cin>>space[i + 1]>>value[i + 1];


		for(int i = 1; i <= n; i++)
		{
			for(int j = 1; j <= b; j++)
			{
				if(j - space[i] < 0)
					array[i][j] = array[i - 1][j];
				else
					array[i][j] = max(array[i - 1][j], array[i - 1][j - space[i]] + value[i]);
			}
		}


		cout<< array[n][b]<< "\n";

		delete [] space;
		delete [] value;

		for(int i = 0; i <= n; i++)
			delete [] array[i];
		delete [] array;

	}


	return 0;
}