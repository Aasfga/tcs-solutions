#include <stdio.h>
#include <stdlib.h>


void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(int *array, int a, int b, int x)
{
	int j = a;

	for(int i = a; i < b; i++)
	{
		if(*(array + i) < x)
		{
			int c = *(array + i);
			*(array + i) = *(array + j);
			*(array + j) = c;

			j++;
		}
	}

	return j;
}

int main()
{
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		int size;
		scanf("%i", &size);

		int *array = (int *) malloc(sizeof(int) * size);

		for(int i = 0; i < size; i++)
		{
			scanf("%i", array + i);
		}

		int asks;
		scanf("%i", &asks);
		int oldI = 0;
		int oldJ = size;
		int oldA = 0;
		int oldB = 2000000000;
		for(int m = 0; m < asks; m++)
		{
			int a, b, i, j;
			scanf("%i %i", &a, &b);
			if(a >= oldA && b <= oldB)
			{
				i = partition(array, oldI, oldJ, a);
				j = partition(array, i, oldJ, b);
			}
			else if(a >= oldB)
			{
				i = partition(array, oldJ, size, a);
				j = partition(array, i, size, b);
			}
			else if(b <= oldA)
			{
				i = partition(array, 0, oldI, a);
				j = partition(array, i, oldI, b);
			}
			else
			{
				i = partition(array, 0, size, a);
				j = partition(array, i, size, b);
			}
			printf("%i %i\n", i, j);
			oldI = i;
			oldJ = j;
			oldA = a;
			oldB = b;
		}

		for(int i = 0; i < size; i++)
		{
			printf("%i ", *(array + i));
		}
		printf("\n");

		free(array);
	}


	return 0;
}