#include <iostream>

using namespace std;

long long power(long long a, long long b)
{
	int x = 1;
	while(b != 0)
	{
		x *= a;
		b--;
	}

	return x;
}

class Stack
{
	int size;
	int nElems;
	long long *array;


public:

	Stack()
	{
		size = 15;
		nElems = 0;
		array = new long long[size];
	}

	Stack(int n)
	{
		size = n;
		nElems = 0;
		array = new long long[size];
	}

	bool empty()
	{
		return nElems == 0;
	}

	bool full()
	{
		return nElems == size;
	}

	void push(const long long &x)
	{
		if(full())
		{
			return;
		}

		array[nElems++] = x;
	}

	long long pop()
	{
		if(empty())
		{
			return 0;
		}


		return array[--nElems];
	}

	long long top()
	{
		return array[nElems - 1];
	}

	void clear()
	{
		nElems = 0;
	}

	void resize(int n)
	{
		if(n <= size)
		{
			return;
		}

		long long *newArray = new long long[n];

		for(int i = 0; i < nElems; i++)
		{
			newArray[i] = array[i];
		}

		delete[] array;
		array = newArray;
	}

	~Stack()
	{
		delete[] array;
	}
};

class OnpExpression
{
	string expression;
	string convertedExpression;
	long long result;

	int priorities(char c)
	{
		if(c == '+' || c == '-')
			return 1;
		else if(c == '*' || c == '/')
			return 2;
		else if(c == '^')
			return 3;
		else
			return 0;
	}

	string llToString(long long x)
	{
		string result = "";

		do
		{
			result = (char) (x % 10 + '0') + result;
			x /= 10;
		} while(x != 0);


		return result;
	}

	long long getNumber(string &input, int i = 0)
	{
		long long x = 0;
		while(isdigit(input[i]))
		{
			x *= 10;
			x += input[i] - '0';
			i++;
		}

		return x;
	}

	string convert()
	{
		string result;
		Stack operators(500000);


		for(int i = 0; i < expression.length(); i++)
		{
			char c = expression[i];
			if(isalpha(c))
				result += c;
			else if(c == '(')
				operators.push(c);
			else if(c == ')')
			{
				while(operators.top() != '(')
					result += operators.pop();
				operators.pop();
			}
			else
			{
				while(!operators.empty())
				{
					if(priorities(c) > priorities(operators.top()))
						break;
					result += (char) operators.pop();
				}

				operators.push(c);
			}
		}

		while(!operators.empty())
			result += (char) operators.pop();


		return result;
	}

	long long calc(string input)
	{
		Stack stack(2000);
		int size = input.length();

		for(int i = 0; i < size; i++)
		{
			if(input[i] == '+')
			{
				long long a = stack.pop();
				long long b = stack.pop();
				stack.push(a + b);
			}
			else if(input[i] == '-')
			{
				long long a = stack.pop();
				long long b = stack.pop();
				stack.push(b - a);
			}
			else if(input[i] == '*')
			{
				long long a = stack.pop();
				long long b = stack.pop();
				stack.push(a * b);
			}
			else if(input[i] == '/')
			{
				long long a = stack.pop();
				long long b = stack.pop();
				if(a == 0)
				{
					stack.push(0);
				}
				else
				{
					stack.push(b / a);
				}

			}
			else if(input[i] == '^')
			{
				long long a = stack.pop();
				long long b = stack.pop();
				if(a < 0)
				{
					stack.push(1);
				}
				else
				{
					stack.push(power(b, a));
				}
			}
			else
			{
				stack.push(getNumber(input, i));
				while(isdigit(input[i]))
					i++;
			}
		}


		return stack.pop();
	}

	string replaceOperands(long long array[26])
	{
		string result = "";

		for(int i = 0; i < convertedExpression.length(); i++)
		{
			if(isalpha(convertedExpression[i]))
			{
				result += llToString(array[convertedExpression[i] - 'a']) + ';';
			}
			else
			{
				result += convertedExpression[i];
			}
		}

		return result;
	}

public:

	OnpExpression(string input, long long array[26])
	{
		expression = input;
		convertedExpression = convert();
		result = calc(replaceOperands(array));
	}

	void changeExpression(string input, long long array[26])
	{
		expression = input;
		convertedExpression = convert();
		result = calc(replaceOperands(array));
	}

	string getExpression()
	{
		return expression;
	}

	string getConvertedExpression()
	{
		return convertedExpression;
	}

	long long getResult()
	{
		return result;
	}
};


int main()
{

	ios_base::sync_with_stdio(false);

	long long array[26];
	int sets;
	cin>> sets;

	while(sets--)
	{

		for(int i = 0; i < 26; i++)
			cin>> array[i];

		int expressionNumber;
		cin>> expressionNumber;

		string text;
		while(expressionNumber--)
		{
			cin>> text;
			OnpExpression expression(text, array);
			cout<< expression.getConvertedExpression() << "\n" << expression.getResult() << "\n";
		}
	}




	return 0;
}