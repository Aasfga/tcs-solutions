#include <iostream>
#include <vector>


using namespace std;

class Graph
{
	int p;
	class Vertex
	{
	public:

		vector<int> e;
		bool win = false;

		vector<int>& edges()
		{
			return e;
		}

		void add_edge(int v)
		{
			e.push_back(v);
		}

	};

	int n;
	Vertex *vertexes;

public:

	Graph(int n): n(n), vertexes(new Vertex[n])
	{}

	void add_edge(int v, int u)
	{
		vertexes[v].add_edge(u);
	}

	friend ostream &operator<<(ostream &os, const Graph &g)
	{
		for(int i = 0; i < g.n; i++)
		{
			if(g.vertexes[i].edges().size() == 0)
				continue;

			os<< i<< ": ";

			for(int j : g.vertexes[i].edges())
				os<< j<< " ";
			os<< "\n";
		}

		return os;
	}

	void set_win()
	{
		for(int i = n - 1; i >= 0; i--)
		{
			if(vertexes[i].edges().empty())
				vertexes[i].win = false;
			else
			{
				for(int j : vertexes[i].e)
				{
					vertexes[i].win |= !vertexes[j].win;
				}
			}
		}
	}


	void print_win()
	{
		for(int i = 0; i < n; i++)
			cout<< (vertexes[i].win ? "W" : "P");
		cout<< "\n";
	}

	~Graph()
	{
		delete [] vertexes;
	}
};



int main()
{
	int sets;
	cin>> sets;

	while(sets--)
	{
		int n;
		cin>> n;
		Graph g(n);

		for(int i = 0; i < n; i++)
		{
			int m;
			cin>>m;

			for(int j = 0; j < m; j++)
			{
				int v;
				cin>> v;
				g.add_edge(i, v);
			}


		}

		g.set_win();
		g.print_win();
	}
	return 0;
}