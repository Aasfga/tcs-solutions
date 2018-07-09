#include <stdio.h>
#include <string.h>

int getValueOfDigit(char digit)
{
	if(digit >= 'A')
	{
		return digit - 'A' + 10;
	}
	else
	{
		return digit - '0';
	}
}

char getDigitFromValue(int digit)
{
	if(digit < 10)
	{
		return digit + '0';
	}
	else
	{
		return digit + 'A' - 10;
	}
}

long long convertToBaseTen(char number[], int base)
{
	int size = strlen(number);

	long long sum = 0;

	for(int i = 0; i < size; i++)
	{
		int digit = getValueOfDigit(number[i]);
		sum = sum * base + digit;
	}

	return sum;
}

void reverse(char array[])
{
	int size = strlen(array);

	for(int i = 0; 2 * i < size; i++)
	{
		char tmp = array[i];
		array[i] = array[size - i - 1];
		array[size - i - 1] = tmp;
	}
}

void convertToBaseQ(long long baseTen, char baseQ[], long long q)
{
	int i;


	if(baseTen == 0)
	{
		baseQ[0] = '0';
		baseQ[1] = 0;
		return;
	}


	for(i = 0; baseTen >= 1; i++)
	{
		baseQ[i] = getDigitFromValue(baseTen % q);
		baseTen /= q;
	}

	baseQ[i] = 0;
	reverse(baseQ);

}

char willFitInRegister(long long number, int size)
{
	unsigned long long checker = ((long long)1 << (long long)size - 1);
	if(number < 0)
	{
		unsigned long long aNumber = (unsigned long long) -number;

		return  aNumber <= checker;
	}
	else
		return number < checker;

}

void negation(char binNumber[])
{
	for(int i = 0; binNumber[i] != 0; i++)
	{
		binNumber[i] = 1 - (binNumber[i] - '0') + '0';
	}
}

void increment(char binNumber[])
{
	int size = strlen(binNumber);
	int i;
	for(i = size - 1; i >= 0 && binNumber[i] != '0'; i--)
	{
		binNumber[i] = '0';
	}

	binNumber[i] = '1';
}

void error(char binNumber[])
{
	char error[] = "ERROR";
	strcpy(binNumber, error);
}

void fitToRegister(char binNumber[], int size)
{
	reverse(binNumber);
	for(int i = strlen(binNumber); i < size; i++)
	{
		binNumber[i] = '0';
	}

	binNumber[size] = 0;
	reverse(binNumber);
}

void _write(long long number, char binNumber[], int size)
{
	if(number >= 0)
	{
		convertToBaseQ(number, binNumber, 2);
		fitToRegister(binNumber, size);
	}
	else
	{
		number *= -1;
		convertToBaseQ(number, binNumber, 2);
		fitToRegister(binNumber, size);
		negation(binNumber);
		increment(binNumber);
	}

}

void write(long long number, char binNumber[], int size)
{
	if(willFitInRegister(number, size))
	{
		_write(number, binNumber, size);
	}
	else
	{
		error(binNumber);
	}
}

long long read(char binNumber[])
{
	long long answer = 1;
	char tmp[65];
	strcpy(tmp, binNumber);
	if(tmp[0] == '1')
	{
		answer *= -1;
		negation(tmp);
		increment(tmp);
	}

	answer *= convertToBaseTen(tmp, 2);

	return answer;
}

int main()
{
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		char task[20];
		scanf("%s", task);
		char array[65];
		long long number;

		if(strcmp(task, "CONVERT") == 0)
		{
			int baseFrom;
			int baseTo;
			scanf("%i %i %s", &baseFrom, &baseTo, array);
			number = convertToBaseTen(array, baseFrom);
			convertToBaseQ(number, array, baseTo);
			printf("%s\n", array);
		}
		else if(strcmp(task, "READ") == 0)
		{
			scanf("%s", array);
			number = read(array);
			printf("%lld\n", number);
		}
		else
		{
			int size;
			scanf("%i %lld", &size, &number);
			write(number, array, size);
			printf("%s\n", array);
		}
	}

	return 0;
}