
#include <string.h>
#include <printf.h>

void printInt(int a)
{
	for(unsigned int i = 1 << 31; i > 0; i >>= 1)
	{
		if(a & i)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}

	printf("\n");
}

void printChar(char a)
{
	for(unsigned char i = 1 << 7; i > 0; i >>= 1)
	{
		if(a & i)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}

	printf("\n");
}

int getSize(int a)
{
	if(a == 0)
	{
		return 0;
	}


	const unsigned int MAX_INT = 1 << 31;

	int i = 0;
	for(i = 0; (a & (MAX_INT >> i)) == 0; i++)
	{}

	return 32 - i;
}

unsigned int moveToLeft(unsigned int a, int size)
{
	return (a << 32 - size);
}

int getNextBit(FILE *file)
{
	static unsigned char buffer = 0;
	static int size = 0;
	int test = 1;
	if(!size)
	{
		test = fread(&buffer, sizeof(char), 1, file);
		size = 8;
	}
	if(test == 0)
		return 0;

	int ans = buffer & (1 << 7);
	buffer <<= 1;
	size--;

	return ans;
}

char getChar(FILE *file)
{
	if(getNextBit(file))
	{
		if(getNextBit(file))
		{
			if(getNextBit(file))
			{
				if(getNextBit(file))
				{
					if(getNextBit(file))
					{
						if(getNextBit(file))
						{
							return 'u';
						}
						else
						{
							return 'j';
						}
					}
					else
					{
						return 'w';
					}
				}
				else
				{
					return 'a';
				}
			}
			else
			{
				if(getNextBit(file))
				{
					if(getNextBit(file))
					{
						return 's';
					}
					else
					{
						return 't';
					}
				}
				else
				{
					return 'i';
				}
			}
		}
		else
		{
			if(getNextBit(file))
			{
				if(getNextBit(file))
				{
					if(getNextBit(file))
					{
						return 'c';
					}
					else
					{
						return 'y';
					}
				}
				else
				{
					return 'o';
				}
			}
			else
			{
				if(getNextBit(file))
				{
					return 'e';
				}
				else
				{
					if(getNextBit(file))
					{
						if(getNextBit(file))
						{
							if(getNextBit(file))
							{
								return 'b';
							}
							else
							{
								return 'g';
							}
						}
						else
						{
							if(getNextBit(file))
							{
								return 'h';
							}
							else
							{
								return 'f';
							}
						}
					}
					else
					{
						if(getNextBit(file))
						{
							if(getNextBit(file))
							{
								if(getNextBit(file))
								{
									return 'd';
								}
							}
							else
							{
								if(getNextBit(file))
								{
									return 'x';
								}
								else
								{
									if(getNextBit(file))
									{
										return 'q';
									}
									else
									{
										return 'v';
									}
								}
							}
						}
						else
						{
							if(getNextBit(file))
							{
								return 'l';
							}
							else
							{
								return 'm';
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if(getNextBit(file))
		{
			if(getNextBit(file))
			{
				if(getNextBit(file))
				{
					return ' ';
				}
				else
				{
					return '.';
				}
			}
			else
			{
				if(getNextBit(file))
				{
					return 'n';
				}
				else
				{
					return 'r';
				}
			}
		}
		else
		{
			if(getNextBit(file))
			{
				if(getNextBit(file))
				{
					return '\n';
				}
				else
				{
					return 'z';
				}
			}
			else
			{
				if(getNextBit(file))
				{
					if(getNextBit(file))
					{
						return 'k';
					}
					else
					{
						return 'p';
					}
				}
			}
		}
	}


	return 0;
}

void writeBits(FILE *file)
{
	char a = 0;
	char buffer = 0;
	unsigned char pointer = 1 << 7;
	while(a != 'q')
	{
		scanf("%c", &a);

		if(pointer == 0)
		{
			fwrite(&buffer, sizeof(char), 1, file);
			pointer = 1 << 7;
			buffer = 0;
		}

		if(a == '1')
		{
			buffer = buffer | pointer;
		}

		pointer >>= 1;
	}

	fwrite(&buffer, sizeof(char), 1, file);
}

int getValue(char array[])
{
	int value = 0;
	for(int i = 0; array[i] != 0; i++)
	{
		value = value * 2 + (array[i] - '0');
	}

	return value;
}

unsigned int encrypt(char aChar)
{
	switch(aChar)
	{
		case 'a':
			return getValue("11110");
		case 'b':
			return getValue("11000111");
		case 'c':
			return getValue("110111");
		case 'd':
			return getValue("110000111");
		case 'e':
			return getValue("11001");
		case 'f':
			return getValue("11000100");
		case 'g':
			return getValue("11000110");
		case 'h':
			return getValue("11000101");
		case 'i':
			return getValue("11100");
		case 'j':
			return getValue("1111110");
		case 'k':
			return getValue("100011");
		case 'l':
			return getValue("11000001");
		case 'm':
			return getValue("11000000");
		case 'n':
			return getValue("10101");
		case 'o':
			return getValue("11010");
		case 'p':
			return getValue("100010");
		case 'q':
			return getValue("1100001001");
		case 'r':
			return getValue("10100");
		case 's':
			return getValue("111011");
		case 't':
			return getValue("111010");
		case 'u':
			return getValue("1111111");
		case 'v':
			return getValue("1100001000");
		case 'w':
			return getValue("111110");
		case 'x':
			return getValue("110000101");
		case 'y':
			return getValue("110110");
		case 'z':
			return getValue("10010");
		case ' ':
			return getValue("10111");
		case '.':
			return getValue("10110");
		case '\n':
			return getValue("10011");
		default:
			return getValue("1000");
	}
}


unsigned int buffer = 0;
unsigned int size = 0;

void writeToFile(FILE *file, char aChar)
{
	unsigned int valueOfChar = encrypt(aChar);
	int sizeOfChar = getSize(valueOfChar) - 1;
	//	printf("%i   ", sizeOfChar);
	//	printInt(valueOfChar);
	valueOfChar = moveToLeft(valueOfChar, sizeOfChar);
	//	printf("%i   ", sizeOfChar);
	//	printInt(valueOfChar);
	valueOfChar >>= size;
	buffer = valueOfChar | buffer;
	//	printf("\t\t");
	//	printInt(buffer);
	size += sizeOfChar;


	if(size >= 8)
	{
		char tmp = 0 | (buffer >> 24);
//	q	printChar(tmp);
		//fwrite(&tmp, sizeof(char), 1, file);
		fprintf(file, "%c", tmp);
		buffer <<= 8;
		size -= 8;
		//printf("\t\t");
	//	printInt(buffer);
	}
}

void flush(FILE *file)
{

	if(size == 0)
		return;


	//printInt(buffer);
	char tmp = 0 | buffer >> 24;
	//	printChar(tmp);
	fwrite(&tmp, sizeof(char), 1, file);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		return 1;
	}

	FILE *first = fopen(argv[1], "rb");
	FILE *second = fopen(argv[2], "wb");

	if(first == NULL)
	{
		fclose(first);
		fclose(second);
		return 1;
	}
	else if(second == NULL)
	{
		fclose(first);
		fclose(second);
		return 1;
	}

	if(strcmp("1", argv[3]) == 0)
	{
		char aChar;
		fscanf(first, "%c", &aChar);

		do
		{
			writeToFile(second, aChar);
			fscanf(first, "%c", &aChar);

		} while(!feof(first));

		flush(second);
	}
	else if(strcmp("2", argv[3]) == 0)
	{
		char aChar;

		for(aChar = getChar(first); aChar != '\0'; aChar = getChar(first))
			fprintf(second, "%c", aChar);
	}

	fclose(first);
	fclose(second);
/*

	second = fopen(argv[2], "r");

	if(strcmp("1", argv[3]) == 0)
	{
		int aChar = fgetc(second);


		while(aChar != -1)
		{

			printf("%i\n", aChar);
			aChar = fgetc(second);
			//printChar(aChar);
		}
	}
	else
	{
		char aChar;

		do
		{
			fscanf(second,"%c", &aChar);
			//printf("%i ", aChar);
			printChar(aChar);
		}while(aChar != '\0');
	}
*/

	return 0;
}