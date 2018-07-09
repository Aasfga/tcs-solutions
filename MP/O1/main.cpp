#include <iostream>
#include <vector>
#include <list>

using namespace std;


class Vertex
{
	friend class Graph;

	vector<int> siblings;
	int degree = 0;


public:

	void incrementDegree()
	{
		degree++;
	}

	void deincrementDegree()
	{
		degree--;
	}

	void addEdge(int x)
	{
		siblings.push_back(x);
	}

	int getDegree()
	{
		return degree;
	}
};

class Graph
{
	vector<Vertex> vertexes;
	int size;
	int maxTerm = 0;

	void setDegrees()
	{
		for(Vertex v : vertexes)
		{
			for(int x : v.siblings)
			{
				vertexes[x].incrementDegree();
			}
		}
	}

	bool areZerosInList(list<int> &l)
	{
		for(int x : l)
		{
			if(vertexes[x].getDegree() == 0)
			{
				return true;
			}
		}

		return false;
	}

	list<int> getInitialList()
	{
		list<int> l;

		for(int i = 1; i < size; i++)
		{
			if(vertexes[i].getDegree() == 0)
			{
				l.push_back(i);
			}
		}

		return l;
	}

public:

	Graph(int size) : vertexes(size + 1), size(size + 1)
	{}

	void addEdge(int x, int y)
	{
		vertexes[x].addEdge(y);
	}

	string getTerms()
	{
		setDegrees();

		list<int> lists[2];
		int l = 0;
		lists[l] = getInitialList();
		int term = 0;
		int printed = 0;
		string ans = "";

		while(!lists[l].empty())
		{
			ans += to_string(lists[l].size()) + " ";
			for(int x : lists[l])
			{
				ans += to_string(x) + " ";
				printed++;

				for(int y : vertexes[x].siblings)
				{
					vertexes[y].degree--;
					if(vertexes[y].degree == 0)
					{
						lists[1 - l].push_back(y);
					}
				}
			}
			ans += "\n";
			lists[l].clear();
			l = 1 - l;
			term++;
		}

		if(printed + 1 != size)
		{
			return "NIE\n";
		}
		else
		{
			return "TAK " + to_string(term) + "\n" + ans;
		}

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
		cout << graph.getTerms();

	}
}
