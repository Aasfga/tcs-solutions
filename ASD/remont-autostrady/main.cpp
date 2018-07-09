#include <iostream>

using namespace std;

int findHole(int *dist, int i, int x)
{
	if(x < 0)
		return i;


	while(dist[i + 1] < x)
		i++;

	return i;
}

int findHole(int *dist, int a, int b, int x)
{
	if(x < 0)
		return a;

	while(a != b)
	{
		int k = (a + b + 1)/2;

		if(dist[k] < x)
			a = k;
		else
			b = k - 1;
	}

	return a;
}



int main()
{
	int sets;
	cin>> sets;

	while(sets--)
	{
		int hole = 0;
		int nHoles;
		long long *array;
		int *dist;
		int *value;
		int roadLength;
		int roadCost;

		cin >> nHoles;
		nHoles++;

		array = new long long[nHoles]{0};
		dist = new int[nHoles]{0};
		value = new int[nHoles]{0};

		for(int i = 1; i < nHoles; i++)
			cin>> dist[i]>> value[i];
		cin>> roadLength>> roadCost;

		for(int i = 1; i < nHoles; i++)
		{
			//hole = findHole(dist, hole, i - 1, dist[i] - roadLength);
			hole = findHole(dist, hole, dist[i] - roadLength);
			array[i] = min(array[i - 1] + value[i], array[hole] + roadCost);
		}


		cout<< array[nHoles - 1]<< "\n";

		delete [] array;
		delete [] dist;
		delete [] value;

	}
	return 0;
}