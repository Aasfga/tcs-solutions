#include <iostream>


int *array;
int *marks;
int aSize = 0;
int toFind = 0;
int k;
void setZero(int *array, int size)
{
	for(int i = 0; i < size; i++)
	{
		array[i] = 0;
	}
}

int firstUnmarked(int i = -1)
{
	for(i = i + 1; i < aSize; i++)
	{
		if(marks[i] == 0)
		{
			break;
		}
	}

	return i;
}

void print()
{

	for(int i = 1; i <= aSize; i++)
	{
		for(int j = 0; j < aSize; j++)
		{
			if(marks[j] == i)
				printf("%i ", array[j]);
		}
	}

	printf("\n");
}

void printPermutations(int level)
{

	for(int i = firstUnmarked(-1); i < aSize; i = firstUnmarked(i))
	{
		marks[i] = level;
		if(level ==  aSize)
		{
			print();
		}

		printPermutations(level + 1);
		marks[i] = 0;
	}

}

void printPermutations()
{
	printPermutations(1);
}

int intLength(int x)
{
	int i;

	for(i = 0; x != 0; x /= 10)
	{
		i++;
	}

	return i;
}

int* intToArray(int x, int size)
{
	int *array = new int[size];

	for(int i = size - 1; i >= 0; i--)
	{
		array[i] = x % 10;

		x /= 10;
	}

	return array;
}

int arrayToInt(int *array, int size)
{
	int ans = 0;

	for(int i = 0 ; i < size; i++)
	{
		ans *= 10;
		ans += array[i];
	}


	return ans;
}

int absVal(int x)
{
	if(x < 0)
		return -x;
	else
		return x;
}

bool isPrime(int x)
{
	if(x == 1)
		return false;
	if(x == 2)
		return true;
	if(x % 2 == 0)
		return false;

	for(int i = 3; i*i <= x; i += 2)
	{
		if(x % i == 0)
			return false;
	}

	return true;
}

bool isPermPrime(int a, int b)
{
	int abs = absVal(a - b);
	return abs % 9 == 0 && abs != 0 && isPrime(abs/9) ;
}


int calcPermVal()
{

	int tmp[aSize];

	for(int i = 1; i <= aSize; i++)
	{
		for(int j = 0; j < aSize; j++)
		{
			if(marks[j] == i)
				tmp[i - 1] = array[j];
		}
	}

	return arrayToInt(tmp, aSize);
}


int findPermPrime(int level = 1)
{
	for(int i = firstUnmarked(); i < aSize && k < 100000000; i = firstUnmarked(i))
	{
		marks[i] = level;
		if(level == aSize)
		{
			int b = calcPermVal();

			k++;
			if(isPermPrime(toFind, b))
				return b;
		}

		int ans = findPermPrime(level + 1);

		if(ans)
			return ans;

		marks[i] = 0;
	}

	return 0;
}


void setVariables(int x)
{
	toFind = x;
	aSize = intLength(x);
	array = intToArray(x, aSize);
	marks = new int[aSize];
	setZero(marks, aSize);
	k = 0;
}

void destroyVariables()
{
	delete array;
	delete marks;
}

int main()
{
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		int x;
		scanf("%i", &x);

		setVariables(x);
		int ans = findPermPrime();

		if(ans == 0)
			printf("NIE\n");
		else
			printf("TAK %i\n", ans);

		destroyVariables();
	}

	return 0;
}