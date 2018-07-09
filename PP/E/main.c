#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void bubbleSort(int *array, int size)
{
	int flag;
	for(int i = size - 1; i > 0; i = flag)
	{
		flag = 0;

		for(int j = 0; j < i; j++)
		{
			if(array[j] > array[j + 1])
			{
				swap(&array[j], &array[j + 1]);
				flag = j;
			}
		}
	}
}

int indexOfMaxElement(int array[], int size)
{
	int max = 0;

	for(int i = 1; i < size; i++)
	{
		if(array[i] > array[max])
		{
			max = i;
		}
	}

	return max;
}

int indexOfMinElement(int array[], int size)
{
	int min = 0;

	for(int i = 1; i < size; i++)
	{
		if(array[i] < array[min])
		{
			min = i;
		}
	}

	return min;
}

void selectionSort(int array[], int size)
{
	for(int i = size; i > 1; i--)
	{
		int max = indexOfMaxElement(array, i);
		swap(&array[i - 1], &array[max]);
	}
}

void insertionSort(int array[], int size)
{
	int min = indexOfMinElement(array, size);
	swap(&array[0], &array[min]);

	for(int i = 1; i < size; i++)
	{
		int value = array[i];
		int j;


		for(j = i; array[j - 1] > value; j--)
		{
			array[j] = array[j - 1];
		}

		array[j] = value;

	}
}

void printArray(int array[], int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%i ", array[i]);
	}
	printf("\n");
}

int sumElements(int array[], int a, int b, int c)
{
	return array[a] + array[b] + array[c];
}

char findAnswer(int array[], int size, int a, int *aB, int *aC)
{
	int b = a;
	int c = size - 1;
	int sum = 1;
	while(b <= c && sum != 0)
	{
		sum = sumElements(array, a, b, c);

		if(sum > 0)
		{
			c--;
		}
		else if(sum < 0)
		{
			b++;
		}
	}

	*aB = b;
	*aC = c;
	return sum == 0;
}

int main()
{
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		int size;
		scanf("%i", &size);

		int array[size];

		for(int i = 0; i < size; i++)
		{
			scanf("%i", &array[i]);
		}

		insertionSort(array, size);
		printArray(array, size);

		int a, b, c;
		char answer;

		for(a = 0; a < size; a++)
		{
			answer = findAnswer(array, size, a, &b, &c);

			if(answer != 0)
			{
				break;
			}
		}

		if(sumElements(array, a, b, c) == 0)
		{
			printf("TAK %i %i %i\n", a, b, c);
		}
		else
		{
			printf("NIE\n");
		}

	}

	return 0;
}