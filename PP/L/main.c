#include <stdio.h>
#include <string.h>

short memory[512];
int pc;
short acc;


void clean()
{
	for(int i = 0; i < 512; i++)
		memory[i] = 0;
}

void printShort(short value)
{
	unsigned short iter = 1 << 15;
	for(; iter > 0; iter >>= 1)
	{
		if(iter & value)
			printf("1");
		else
			printf("0");
	}

	printf("\n");
}

short getCommand()
{
	char task[10];
	scanf("%s", task);
	short command = 0;

	if(strcmp(task, "NULL") == 0)
		command = 0;
	else if(strcmp(task, "STOP") == 0)
		command = 1;
	else if(strcmp(task, "LOAD") == 0)
		command = 2;
	else if(strcmp(task, "STORE") == 0)
		command = 3;
	else if(strcmp(task, "JUMP") == 0)
		command = 4;
	else if(strcmp(task, "JNEG") == 0)
		command = 5;
	else if(strcmp(task, "JZERO") == 0)
		command = 6;
	else if(strcmp(task, "ADD") == 0)
		command = 8;
	else if(strcmp(task, "SUB") == 0)
		command = 9;
	else if(strcmp(task, "AND") == 0)
		command = 12;
	else if(strcmp(task, "OR") == 0)
		command = 13;
	else if(strcmp(task, "NOT") == 0)
		command = 14;
	else if(strcmp(task, "SHL") == 0)
		command = 10;
	else if(strcmp(task, "SHR") == 0)
		command = 11;

	command <<= 11;

	return command;
}

short getAddressType()
{
	char addressType[2];
	scanf("%s", addressType);
	short addressValue;

	if(strcmp(addressType, ".") == 0)
		addressValue = 0;
	else if(strcmp(addressType, "@") == 0)
		addressValue = 1;
	else if(strcmp(addressType, "*") == 0)
		addressValue = 2;
	else
		addressValue = 3;

	addressValue <<= 9;


	return addressValue;
}

short getValue()
{
	int helper;
	scanf("%i", &helper);
	short value = 0;

	if(helper < 0)
	{
		value = (short)1 << 15;
		helper *= -1;
	}

	value = value | helper;

	return value;
}

short getTask()
{
	short value = 0;
	value |= getCommand();
	value |= getAddressType();
	value |= getValue();

	return value;
}

short getData()
{
	char tester[2];
	scanf("%1s", tester);
	short value = 0;
	if((tester[0] == '-') || (tester[0] >= '0' && tester[0] <= '9'))
	{
		ungetc(tester[0], stdin);
		scanf("%hd", &value);
	}
	else
	{
		ungetc(tester[0], stdin);
		value = getTask();
	}


	return value;
}

void saveNextLine()
{
	int index;
	scanf("%i: ", &index);

	memory[index] = getData();
}

void newSet(int lines)
{
	while(lines--)
	{
		saveNextLine();
	}
}

short retriveCommand(short task)
{
	short command = task & (short) 30720;
	command >>= 11;
	return command;
}

short retriveValue(short task)
{
	short value = task & (short) 511;
	if(task & (1 << 15))
		value *= -1;

	return value;
}

short retriveAddressType(short task)
{
	short address = task & (short) 1536;
	address >>= 9;
	return address;
}

short finalValue(short addressType, short value)
{
	switch(addressType)
	{
		case 0:
			return value;
		case 1:
			return memory[value];
		case 2:
			return memory[memory[value]];
		case 3:
			return memory[acc + value];
		default:
			return value;
	}
}

void null(short value)
{}

void stop(short value)
{
	pc = -100;
}

void load(short value)
{
	acc = value;
}

void store(short value)
{
	memory[value] = acc;
}

void jump(short value)
{
	pc = value;
}

void jneg(short value)
{
	if(acc < 0)
		jump(value);
}

void jzero(short value)
{
	if(acc == 0)
		jump(value);
}

void add(short value)
{
	acc += value;
}

void sub(short value)
{
	acc -= value;
}

void and(short value)
{
	acc &= value;
}

void or(short value)
{
	acc |= value;
}

void not(short value)
{
	acc = ~acc;
}

void shl(short value)
{
	acc <<= value;
}

void shr(short value)
{
	acc >>= value;
}

void run()
{
	void (*fun[15])(short value) = {
			null,
	        stop,
	        load,
	        store,
			jump,
	        jneg,
	        jzero,
	        stop,
	        add,
	        sub,
	        shl,
	        shr,
	        and,
	        or,
	        not,
	};


	pc = 0;
	acc = 0;
	short ir;

	while(pc >= 0)
	{
		ir = memory[pc];
		pc++;
		fun[retriveCommand(ir)](finalValue(retriveAddressType(ir), retriveValue(ir)));
	}

}

int main()
{
	int tasks, answers;
	int sets;
	scanf("%i", &sets);

	while(sets--)
	{
		clean();
		scanf("%i %i", &tasks, &answers);
		newSet(tasks);
		int array[answers];

		for(int i = 0; i < answers; i++)
		{
			scanf("%i", &array[i]);
		}

		run();


		for(int i = 0; i < answers; i++)
		{
			printf("%i ", memory[array[i]]);
		}
		printf("\n");
	}

	return 0;
}