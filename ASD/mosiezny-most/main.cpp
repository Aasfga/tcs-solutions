#include <iostream>

using namespace std;

int main()
{
	int sets;
	cin >> sets;

	while(sets--)
	{
		int nPillars;
		int *pillars;
		long long *array;

		cin >> nPillars;
		nPillars++;
		pillars = new int[nPillars]{0};
		array = new long long[nPillars]{0};


		for(int i = 1; i < nPillars; i++)
			cin >> pillars[i];

		if(nPillars > 1)
			array[1] = pillars[1];
		if(nPillars > 2)
			array[2] = pillars[2] + pillars[1];

		for(int i = 3; i < nPillars; i++)
		{
			array[i] = max(array[i - 1],
			               max(array[i - 2] + pillars[i],
			                   array[i - 3] + pillars[i] + pillars[i - 1])
			              );
		}

		cout << array[nPillars - 1] << "\n";


		delete[] pillars;
		delete[] array;
	}

	return 0;
}