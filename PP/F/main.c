#include <string.h>
#include <stdio.h>

char toLowerCase(char a)
{
	return (char) (a < 97 && a != '\0' ? a + 32 : a);
}

void getAlphabet(int alphabet[])
{
	char aChar[2];
	for(int i = 0; i < 26; i++)
	{
		scanf("%s", aChar);

		alphabet[aChar[0] - 97] = i;
	}
}

int compareChars(int alphabet[], char a, char b)
{
	a = toLowerCase(a);
	b = toLowerCase(b);

	if(a == '\0' && b != '\0')
	{
		return -1;
	}
	else if(a != '\0' && b == '\0')
	{
		return 1;
	}
	else if(a == '\0' /*&& b == '\0'*/)
	{
		return 0;
	}

	int valueOfA = alphabet[a - 'a'];
	int valueOfB = alphabet[b - 'a'];

	return valueOfA - valueOfB;
}

int compareStrings(int alphabet[], char a[], char b[])
{
	int i = -1;
	int x;


	do
	{
		i++;
		x = compareChars(alphabet, a[i], b[i]);

	} while(x == 0 && a[i] != '\0' && b[i] != '\0');


	return x;
}

int minIndex(int alphabet[], char strings[][121], int size)
{
	int min = 0;

	for(int i = 1; i < size; i++)
	{
		if(compareStrings(alphabet, strings[min], strings[i]) > 0)
		{
			min = i;
		}
	}

	return min;
}

void swap(char first[], char second[])
{
	char tmp[121];
	char secTmp[121];
	strcpy(tmp, first);
	strcpy(secTmp, second);
	strcpy(second, tmp);
	strcpy(first, secTmp);
}

void insertionSort(int alphabet[], char strings[][121], int size)
{
	int min = minIndex(alphabet, strings, size);
	swap(strings[min], strings[0]);

	for(int i = 1; i < size; i++)
	{
		char value[121];
		strcpy(value, strings[i]);
		int j;

		for(j = i; compareStrings(alphabet, strings[j - 1], value) > 0; j--)
		{
			strcpy(strings[j], strings[j - 1]);
		}

		strcpy(strings[j], value);
	}

}

int main()
{
	int alphabet[26];
	int sets, stringsSize, numberAlphabets;
	scanf("%i", &sets);

	while(sets--)
	{
		scanf("%i %i", &stringsSize, &numberAlphabets);
		char strings[stringsSize][121];

		for(int i = 0; i < stringsSize; i++)
		{
			scanf("%s", strings[i]);
		}

		for(int i = 0; i < numberAlphabets; i++)
		{
			getAlphabet(alphabet);
			insertionSort(alphabet, strings, stringsSize);

			for(int j = 0; j < stringsSize; j++)
			{
				printf("%s ", strings[j]);
			}
			printf("\n");

		}
	}

	return 0;
}