#include "solution.h"
#include <iostream>

using namespace std;


int main()
{
	ios_base::sync_with_stdio(false);
	int z, n;
	string s, t;
	myPair<int> x;
	myPair<stackMin<int> > p;
	p.first.push(5);
	p.second.push(7);
	p.first.push(15);
	p.second.push(3);
	cout << p.first.top() << " " << p.second.top() << endl;
	cout << p.first.min() << " " << p.second.min() << endl;
	cin >> z;
	while(z--)
	{
		cin >> n >> s >> t;
		if(t == "STACK")
		{
			switch(s[0])
			{
				case 'I':
				{
					stackMin<int> S;
					solveStack(S, n);
					break;
				}
				case 'S':
				{
					stackMin<string> S;
					solveStack(S, n);
					break;
				}
				case 'D':
				{
					stackMin<double> S;
					solveStack(S, n);
					break;
				}
			}
		}
		else
		{             //if (t == "QUEUE")
			switch(s[0])
			{
				case 'I':
				{
					queueMin<int> Q;
					solveQueue(Q, n);
					break;
				}
				case 'S':
				{
					queueMin<string> Q;
					solveQueue(Q, n);
					break;
				}
				case 'D':
				{
					queueMin<double> Q;
					solveQueue(Q, n);
					break;
				}
			}
		}
	}

	return 0;
}
