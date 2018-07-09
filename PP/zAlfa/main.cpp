/*
int main()
{
	ios_base::sync_with_stdio(false);


	number a;
	number b;

	std::cin>> a>> b;

	cout<< a << "\n"<< b<< "\n";
	a *= b;
	cout<< a<< "\n"<< b;
	cout<< "\n"<< (a < b);


	/*
	ios_base::sync_with_stdio(false);

	int sets;
	cin>> sets;

	while(sets--)
	{
		char operation;
		cin>> operation;
		number a, b;

		if(operation == '+')
		{
			a += b;
			cout<< to_string(a);
		}
		else if(operation == '-')
		{
			a -= b;
			cout << to_string(a);
		}
		else if(operation == '*')
		{
			a *= b;
			cout<< to_string(a);
		}
		else
		{
			cout<< a.cmp(b);
		}

		cout<< "\n";

	}

	return 0;




}

*/

#include<iostream>
#include<string>

using namespace std;

#include "solution.h"

int main()
{
	string s;
	number C;
	number D("123");
	//cout << C << " " << D << endl;
	int z;
	cin >> z;
	while(z--)
	{
		number A;
		number B;
		//cout<< A<<"     "<< B<< "\n";
		cin >> s >> A >> B;
		//cout<< A<<"     "<< B<< "\n";
		switch(s[0])
		{
			case '+': A += B;
				cout << A << endl;
				break;
			case '-':A -= B;
				cout << A << endl;
				break;
			case '*':A *= B;
				cout << A << endl;
				break;
			case '<' : cout << (A < B) << endl;
		}
	}
	return 0;
}