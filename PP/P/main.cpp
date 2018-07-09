#include <iostream>

int isOdd(int a)
{
	return a & 1;
}

int NwdRec(int a, int b)
{
	if(!isOdd(a) && !isOdd(b))
	{
		return NwdRec(a >> 1, b >> 1) << 1;
	}
	else if(a == 0)
		return b;
	else if(b == 0)
		return a;
	else if(!isOdd(a))
		return NwdRec(a >> 1, b);
	else if(!isOdd(b))
		return NwdRec(a, b >> 1);
	else if(a < b)
		return NwdRec((b-a) >> 1, a);
	else
		return NwdRec((a - b) >> 1, b);
}

int NwdIter(int a, int b)
{
	int nwd = 0;

	while(!isOdd(a) && !isOdd(b) && a != 0 && b != 0)
	{
		a >>= 1;
		b >>= 1;
		nwd++;
	}

	while(b != 0)
	{


		while(!isOdd(a))
			a >>= 1;
		while(!isOdd(b))
			b >>= 1;


		if( a >= b )
		{
			a = (a - b) >> 1;
		}
		else
		{
			int tmp = (b - a) >> 1;
			b = a;
			a = tmp;
		}

		if(a == 0)
		{
			a = b;
			b = 0;
		}
	}

	return a << nwd;
}


int main()
{
	int sets;
	int a,b;

	scanf("%i", &sets);


	while(sets--)
	{
		scanf("%i %i", &a, &b);

		printf("%i\n", NwdIter(a, b));
	}

	return 0;
}