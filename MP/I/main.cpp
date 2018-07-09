#include <iostream>
#include <stack>
#include <queue>

using namespace std;

class Tree
{
	int size;
	int *child;
	int *brother;
	int *distFather;
	long long *distAll;
	int *subSize;
	long long min;
	pair<int, int> ans;


	void correctMin(int n)
	{
		if(distAll[n] < min)
		{
			min = distAll[n];
			ans.first = n;
			ans.second = -1;
		}
		else if(distAll[n] == min)
		{
			ans.second = n;
		}
	}

	void addAllChildren(queue<pair<int, int> > &nodes, int n)
	{
		pair<int, int> p;
		if(child[n] == -1)
		{
			return;
		}

		p.first = child[n];
		p.second = n;
		nodes.push(p);

		int node = child[n];
		node = brother[node];

		while(node != -1)
		{
			p.first = node;
			nodes.push(p);
			node = brother[node];
		}
	}

	void setZero(int *array)
	{
		for(int i = 0; i < size; i++)
		{
			array[i] = 0;
		}
	}

	void setZero(long long *array)
	{
		for(int i = 0; i < size; i++)
		{
			array[i] = 0;
		}
	}

	void createArrays()
	{
		child = new int[size];
		brother = new int[size];
		distFather = new int[size];
		distAll = new long long[size];
		subSize = new int[size];

		setZero(distAll);
		setZero(subSize);
	}

	void readArrays()
	{
		for(int i = 0; i < size; i++)
		{
			cin >> child[i] >> brother[i] >> distFather[i];
		}
	}

	void calcSizes()
	{
		stack<pair<int, int> > first;
		stack<pair<int, int> > second;

		pair<int, int> p(0, -1);
		first.push(p);

		while(!first.empty())
		{
			p = first.top();
			first.pop();
			second.push(p);

			if(child[p.first] != -1)
			{
				pair<int, int> tmp(child[p.first], p.first);

				first.push(tmp);
			}
			if(brother[p.first] != -1)
			{
				pair<int, int> tmp(brother[p.first], p.second);
				first.push(tmp);
			}
		}

		while(!second.empty())
		{
			p = second.top();
			second.pop();

			subSize[p.first]++;

			if(p.second != -1)
			{
				subSize[p.second] += subSize[p.first];
			}
		}

	}

	void calcRootDist()
	{
		stack<pair<int, int> > nodes;
		pair<int, int> p(0, 0);
		nodes.push(p);

		while(!nodes.empty())
		{
			p = nodes.top();
			nodes.pop();
			distAll[0] += p.second;

			if(child[p.first] != -1)
			{
				pair<int, int> tmp(child[p.first], p.second + distFather[child[p.first]]);
				nodes.push(tmp);
			}
			if(brother[p.first] != -1)
			{
				pair<int, int> tmp(brother[p.first], p.second - distFather[p.first] + distFather[brother[p.first]]);
				nodes.push(tmp);
			}
		}
	}

	void calcAllDist()
	{
		queue<pair<int, int> > nodes;
		addAllChildren(nodes, 0);

		ans.first = 0;
		ans.second = -1;
		min = distAll[0];

		pair<int, int> p;
		while(!nodes.empty())
		{
			p = nodes.front();
			if(!nodes.empty())
			{
				nodes.pop();
			}

			int crr = p.first;
			int parent = p.second;
			distAll[crr] = distAll[parent] + (size - 2 * subSize[crr]) * distFather[crr];
			if(distAll[crr] <= distAll[parent])
				addAllChildren(nodes, crr);
			correctMin(crr);
		}

	}

public:

	Tree()
	{
		cin >> size;
		createArrays();
		readArrays();
		calcSizes();
		calcRootDist();
		calcAllDist();


	}

	void printData()
	{
		for(int i = 0; i < size; i++)
		{
			cout << "\n" << child[i] << " " << brother[i] << " " << distFather[i] << " " << subSize[i] << " "
			     << distAll[i];
		}
	}

	void print()
	{
		if(ans.second == -1)
			cout<< ans.first<< "\n";
		else if(ans.first > ans.second)
			cout<< ans.second<< " "<< ans.first<< "\n";
		else
			cout<< ans.first<< " "<< ans.second<< "\n";
	}

	~Tree()
	{
		delete[] child;
		delete[] brother;
		delete[] distFather;
		delete[] distAll;
		delete[] subSize;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);

	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		Tree tree;
		tree.print();
	}


	return 0;
}