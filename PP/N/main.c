#include <stdio.h>
#include <stdlib.h>

void printThreeDigits(int a)
{
	int i = a % 10;
	int j = (a / 10) % 10;
	int k = (a / 100) % 10;
	printf("%i%i%i ", k, j, i);
}

struct matrix
{
	int **matrix;

	int width;
	int height;
};

void setZero(struct matrix *aMatrix)
{
	for(int i = 0; i < aMatrix->height; i++)
	{
		for(int j = 0; j < aMatrix->width; j++)
		{
			*(*(aMatrix->matrix + i) + j) = 0;
		}
	}
}

void printMatrix(struct matrix *aMatrix)
{
	if(aMatrix == NULL)
	{
		printf("ERROR\n");
		return;
	}

	for(int i = 0; i < aMatrix->height; i++)
	{
		for(int j = 0; j < aMatrix->width; j++)
		{
			printThreeDigits(*(*(aMatrix->matrix + i) + j));
			//printf("%i ", *(*(aMatrix -> matrix + i) + j));
		}
		printf("\n");
	}
}

void readMatrix(struct matrix *aMatrix)
{
	for(int i = 0; i < aMatrix->height; i++)
	{
		for(int j = 0; j < aMatrix->width; j++)
		{
			scanf("%i", (*(aMatrix->matrix + i) + j));
		}
	}
}

struct matrix *createMatrix(int height, int width)
{
	struct matrix *newMatrix = malloc(sizeof(struct matrix));
	newMatrix->width = width;
	newMatrix->height = height;

	newMatrix->matrix = malloc(sizeof(int *) * height);

	for(int i = 0; i < height; i++)
	{
		*(newMatrix->matrix + i) = malloc(sizeof(int) * width);
	}

	return newMatrix;
}

struct matrix *createMatrix_()
{
	int height;
	int width;
	scanf("%i %i", &height, &width);
	struct matrix *answer = createMatrix(height, width);
	readMatrix(answer);
	return answer;
}

void destroyMatrix(struct matrix *aMatrix)
{
	if(aMatrix == NULL)
	{
		return;
	}

	for(int i = 0; i < aMatrix->height; i++)
	{
		free(*(aMatrix->matrix + i));
	}

	free(aMatrix->matrix);

	free(aMatrix);
}

struct matrix *mult(struct matrix *first, struct matrix *second)
{
	if(first->width != second->height)
	{
		return NULL;
	}

	struct matrix *answer = createMatrix(first->height, second->width);
	setZero(answer);

	for(int i = 0; i < first->height; i++)
	{
		for(int j = 0; j < first->width; j++)
		{
			for(int k = 0; k < second->width; k++)
			{
				*(*(answer->matrix + i) + k) += *(*(first->matrix + i) + j) * *(*(second->matrix + j) + k);
				*(*(answer -> matrix + i) + k) %= 1000;
			}
		}
	}

	return answer;
}

void setID(struct matrix *aMatrix)
{
	if(aMatrix->width != aMatrix->height)
	{
		return;
	}

	setZero(aMatrix);

	for(int i = 0; i < aMatrix->height; i++)
	{
		*(*(aMatrix->matrix + i) + i) = 1;
	}
}

struct matrix *copy(struct matrix *aMatrix)
{
	struct matrix *answer = createMatrix(aMatrix->height, aMatrix->width);

	for(int i = 0; i < aMatrix->height; i++)
	{
		for(int j = 0; j < aMatrix->width; j++)
		{
			*(*(answer->matrix + i) + j) = *(*(aMatrix->matrix + i) + j);
		}
	}

	return answer;
}

struct matrix *naPewnoNiePotega(struct matrix *aMatrix, int pow)
{
	struct matrix *answer = createMatrix(aMatrix->height, aMatrix->width);
	setID(answer);
	struct matrix *powerer = copy(aMatrix);

	for(; pow > 0; pow >>= 1)
	{
		if(pow & 1)
		{
			struct matrix *tmp = mult(answer, powerer);
			destroyMatrix(answer);
			answer = tmp;
		}
		struct matrix *tmp = powerer;
		powerer = mult(powerer, powerer);
		destroyMatrix(tmp);
	}

	destroyMatrix(powerer);

	return answer;
}

int main()
{
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		int width, power;
		scanf("%i %i", &width, &power);

		struct matrix *aMatrix = createMatrix(width, width);
		readMatrix(aMatrix);
		struct matrix *answer = naPewnoNiePotega(aMatrix, power);
		printMatrix(answer);
		destroyMatrix(aMatrix);
		destroyMatrix(answer);
	}


	return 0;
}