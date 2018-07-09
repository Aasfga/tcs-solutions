#include <iostream>

#define MAX_VALUE 9223372036854775807;

using namespace std;

struct point
{
	int x;
	int y;


	int calcDist(const point &p)
	{
		return (y - p.y) * (y - p.y) + (x - p.x) * (x - p.x);
	}

};



void giveData(point *&array, int n)
{
	array = new point[n];
	for(int i = 0; i < n; i++)
		cin>> array[i].x>> array[i].y;
}



void allocateArrays(long long **&first, long long **&second, int n)
{
	first = new long long*[n];
	second = new long long*[n];

	for(int i = 0; i < n; i++)
	{
		first[i] = new long long[n]{0};
		second[i] = new long long[n]{0};
	}
}

void freeArrays(long long**first, long long**second, int n)
{
	for(int i = 0; i < n; i++)
	{
		delete [] first[i];
		delete [] second[i];
	}

	delete [] first;
	delete [] second;
}

void printDiagonals(long long **cost, long long **diagonals, int i, int j)
{
	if(i < 0 || j < 0)
		return;
	if(cost[i][j] == 0)
		return;
	else if(diagonals[i][j] == i - 1)
	{
		cout<< j<< " "<< i - 1<< " ";
		printDiagonals(cost, diagonals, i - 1, j);
	}
	else if(diagonals[i][j] == j + 1)
	{
		cout<< j + 1<< " "<< i<< " ";
		printDiagonals(cost, diagonals, i, j + 1);
	}
	else
	{
		long long k = diagonals[i][j];
		if(i - 1 != k)
		{
			cout<< k<< " "<< i<< " ";
			printDiagonals(cost, diagonals, i, k);
		}
		if(j + 1 != k)
		{
			cout<< j<< " "<< k<< " ";
			printDiagonals(cost, diagonals, k, j);
		}
	}
}

int main()
{
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		int n;
		cin>> n;

		point *points;
		long long **cost;
		long long **diagonals;
		giveData(points, n);
		allocateArrays(cost, diagonals, n);


		for(int i = 1; i < n; i++)
		{
			for(int j = i - 1; j >= 0; j--)
			{
				if(i - j <= 2)
					continue;

				long long crrCost = MAX_VALUE;
				long long crrPoint = -1;
				long long oldCost = crrCost;
				point pI = points[i];
				point pJ = points[j];

				for(int k = j + 2; k <= i - 2; k++)
				{
					point pK = points[k];

					oldCost = crrCost;
					crrCost = min(cost[i][k] + cost[k][j] + pI.calcDist(pK) + pJ.calcDist(pK), crrCost);
					if(oldCost != crrCost)
						crrPoint = k;
				}

				if(j + 1 < n)
				{
					point p = points[j + 1];
					oldCost = crrCost;
					crrCost = min(cost[i][j + 1] + p.calcDist(pI), crrCost);
					if(oldCost != crrCost)
						crrPoint = j + 1;
				}
				if(i - 1 >= 0)
				{
					point p = points[i - 1];
					oldCost = crrCost;
					crrCost = min(cost[i - 1][j] + p.calcDist(pJ), crrCost);
					if(oldCost != crrCost)
						crrPoint = i - 1;
				}

				cost[i][j] = crrCost;
				diagonals[i][j] = crrPoint;
			}
		}

		cout<< cost[n - 1][0]<< " ";
		printDiagonals(cost, diagonals, n - 1, 0);
		cout<< "\n";

		freeArrays(cost, diagonals, n);
	}
}

