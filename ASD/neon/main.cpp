#include <iostream>
#include <algorithm>

using namespace std;


int** allocArray(int n, int m)
{
	int **array = new int*[n];

	for(int i = 0; i < n; i++)
		array[i] = new int[m]{0};

	return array;
}

void freeArray(int **array, int n)
{
	for(int i = 0; i < n; i++)
		delete [] array[i];
	delete [] array;
}


string retrieveString(int** array, string first, string second)
{
	int n = first.length() - 1;
	int m = second.length() - 1;
	string result = "";

	while(n * m != 0)
	{
		if(first[n] == second[m])
		{
			result += first[n];
			n--;
			m--;
		}
		else if(array[n - 1][m] > array[n][m - 1])
			n--;
		else
			m--;
	}

	std::reverse(result.begin(), result.end());
	return result;
}

string lcs(string first, string second)
{
	int n = first.length();
	int m = second.length();
	int **array = allocArray(n + 1, m + 1);
	first = " " + first;
	second = " " + second;


	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= m; j++)
		{
			if(first[i] == second[j])
				array[i][j] = array[i - 1][j - 1] + 1;
			else
				array[i][j] = max(array[i - 1][j], array[i][j - 1]);
		}
	}

	string result = retrieveString(array, first, second);
	freeArray(array, n + 1);

	return result;
}


int main()
{
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		string first;
		string second;
		cin>> first>> second;
		string result = lcs(first, second);
		cout<< result.length()<< "\n"<< result<< "\n";
	}

	return 0;
}