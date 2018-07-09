#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;


unsigned long p = 99409;
unsigned long q = 81071;
unsigned long z = 100000007;

unsigned long hashFunction(const string &line, int &i)
{
	i++;
	vector<unsigned long> children;
	while(true)
	{
		if(line[i] == '(')
			children.push_back(hashFunction(line, i));
		if(line[i] == ')')
		{
			i++;
			break;
		}
	}
	sort(children.begin(), children.end());
	unsigned long hash = p;
	unsigned long prime = p;

	for(int j = 0; j < children.size(); j++)
	{
		prime *= p;
		hash *= (children[j]*prime)^q;
	}


	return hash;
}

int main()
{
	ios_base::sync_with_stdio(false);
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		int sets;
		cin>> sets;
		vector<unsigned long> hashes;

		while(sets--)
		{
			string line;
			int i = 0;
			cin>> line;
			hashes.push_back(hashFunction(line, i));
		}
		sort(hashes.begin(), hashes.end());

		int ans = 1;
		unsigned long crrHash = hashes[0];

		for(int i = 0; i < hashes.size(); i++)
		{
			if(crrHash != hashes[i])
			{
				ans++;
				crrHash = hashes[i];
			}
		}
		cout<< ans<< "\n";
	}
	return 0;
}