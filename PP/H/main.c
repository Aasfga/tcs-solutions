#include <stdio.h>


int isBlank(int aChar)
{
	return aChar == ' ' || aChar == '\n' || aChar == '\r';
}

void skipBlank(FILE *file)
{
	int tmpChar;

	for(tmpChar = fgetc(file); isBlank(tmpChar); tmpChar = fgetc(file))
	{
	}

	ungetc(tmpChar, file);
}

void skipComment(FILE *file)
{
	for(int aChar = fgetc(file);; aChar = fgetc(file))
	{
		if(aChar == '*')
		{
			aChar = fgetc(file);

			if(aChar == '/')
			{
				break;
			}
		}
		if(aChar == '/')
		{
			aChar = fgetc(file);

			if(aChar == '*')
			{
				skipComment(file);
			}
		}
	}
}

int isComment(FILE *file)
{
	int aChar = fgetc(file);

	if(aChar == '*')
	{
		return 1;
	}
	else
	{
		ungetc(aChar, file);
	}

	return 0;
}

void skipUnwanted(FILE *file)
{
	for(int aChar = fgetc(file);; aChar = fgetc(file))
	{
		if(isBlank(aChar))
		{
			skipBlank(file);
		}
		else if(aChar == '/')
		{
			if(isComment(file))
			{
				skipComment(file);
			}
			else
			{
				ungetc(aChar, file);
				break;
			}
		}
		else
		{
			ungetc(aChar, file);
			break;
		}
	}
}

int getNextCorrectChar(FILE *file)
{
	int aChar = fgetc(file);

	if(!isBlank(aChar) && aChar != '/')
	{
		return aChar;
	}
	else if(isBlank(aChar))
	{
		if(aChar == '\r')
			aChar = '\n';
		skipBlank(file);
		skipUnwanted(file);
		return aChar;
	}
	else
	{
		for(aChar; aChar == '/' && isComment(file); aChar = fgetc(file))
		{
			skipComment(file);
		}

		if(isBlank(aChar))
		{
			skipUnwanted(file);
		}

		return aChar;
	}

}

int main(int argc, char *argv[])
{
	if(argc != 4)
		return 1;

	FILE *first = fopen(argv[1], "r");
	FILE *second = fopen(argv[2], "r");
	FILE *output = fopen(argv[3], "w");

	if(first == NULL || second == NULL || output == NULL)
	{
		fclose(first);
		fclose(second);
		fclose(output);

		return 1;
	}

	int a;
	int b;
	int line = 1;
	int col = 1;

	while(1)
	{
		a = getNextCorrectChar(first);
		b = getNextCorrectChar(second);

		if(isBlank(a) && isBlank(b))
		{
			if(a == '\n')
			{
				col = 1;
				line++;
			}
			else
				col++;

			fprintf(output, "%c", a);
		}
		else if(a == -1 || b == -1)
		{
			break;
		}
		else if(a == b)
		{
			fprintf(output, "%c", a);
			col++;
		}
		else if(a != b)
		{
			break;
		}
	}

	if(a != b)
	{
		fprintf(output, "\nlinia %i, numer %i: ", line, col);

		if(a == -1)
		{
			fprintf(output, "znak1: EOF, znak2: %c", b);
		}
		else if(b == -1)
		{
			fprintf(output, "znak1: %c, znak2: EOF", a);
		}
		else
		{
			fprintf(output, "znak1: %c, znak2: %c", a, b);
		}
	}


	fclose(first);
	fclose(second);
	fclose(output);
	return 0;
}