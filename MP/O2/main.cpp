#include <iostream>
#include <vector>
#include <list>
#include <stack>

using namespace std;

string toString(list<int> &l)
{
	string ans;
	for(int x : l)
		ans += to_string(x) + " ";


	return ans + "\n";
}

class Vertex
{
	friend class Graph;

	vector<int> siblings;
	int degree = 0;
	int term = 0;
	char colour = 0;

public:

	void addEdge(int x)
	{
		siblings.push_back(x);
	}
};


class Graph
{
	int size;
	int maxTerm = 0;
	vector<Vertex> vertexes;

	void setDegrees()
	{
		for(int i = 1; i < size; i++)
		{

			for(int x : vertexes[i].siblings)
			{
				vertexes[x].degree++;
			}
		}
	}


public:

	Graph(int size) : vertexes(size + 1), size(size + 1)
	{
		for(int i = 1; i < this->size; i++)
		{
			vertexes[0].addEdge(i);
		}
	}

	void addEdge(int x, int y)
	{
		vertexes[x].addEdge(y);
	}

	bool getSort(int i, stack<int, vector<int>> &sort)
	{
		//cout<< toString(sort)<< endl;
		vertexes[i].colour = 1;

		for(int x : vertexes[i].siblings)
		{
			if(vertexes[x].colour == 1)
			{
				return false;
			}
			else if(vertexes[x].colour == 0)
			{
				if(!getSort(x, sort))
				{
					return false;
				}
			}
		}

		vertexes[i].colour = 2;
		sort.push(i);
		return true;
	}


	void setTerms(int i)
	{
		maxTerm = max(maxTerm, vertexes[i].term);
		int *jTerm;
		int *iTerm;
		int x;
		for(int j : vertexes[i].siblings)
		{
			x = j;
			iTerm = &vertexes[i].term;
			jTerm = &vertexes[j].term;
			vertexes[j].term = max(vertexes[j].term, vertexes[i].term + 1);
		}
	}

	void printTerms()
	{
		setDegrees();
		stack<int, vector<int>> sort;
		if(!getSort(0, sort))
		{
			cout<< "NIE\n";
			return;
		}
		sort.pop();
		//cout<< toString(sort);
		while(!sort.empty())
		{
			setTerms(sort.top());
			sort.pop();
		}

		string *terms = new string[maxTerm + 1];
		int *howMany = new int[maxTerm + 1]{0};

		for(int i = 1; i < size; i++)
		{
			terms[vertexes[i].term] += to_string(i) + " ";
			howMany[vertexes[i].term]++;

		}


		cout<< "TAK "<< maxTerm + 1<< "\n";
		for(int i = 0; i <= maxTerm; i++)
			cout<< howMany[i]<< " "<< terms[i]<< "\n";

		delete [] terms;
		delete [] howMany;
	}
};

int main()
{

	int sets;
	cin >> sets;


	while(sets--)
	{
		int size;
		int tasks;
		cin >> size >> tasks;

		Graph graph(size);

		for(int i = 0; i < tasks; i++)
		{
			int x;
			char arrow;
			int y;

			cin >> x >> arrow >> y;
			graph.addEdge(x, y);
		}
		graph.printTerms();

	}
}