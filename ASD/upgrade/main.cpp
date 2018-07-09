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
		int path[2]{-1, -1};

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

	int _calc_path(Vertex &v)
	{
		if(v.edges().size() == 0)
		{
			v.path[p] = 1;
			return 1;
		}

		v.path[p] = 0;

		for(int i : v.edges())
		{
			if(vertexes[i].path[p] != -1)
				v.path[p] = max(v.path[p], 1 + vertexes[i].path[p]);
			else
				v.path[p] = max(v.path[p], 1 + _calc_path(vertexes[i]));
		}

		return v.path[p];
	}

public:

	Graph(int n): n(n), vertexes(new Vertex[n])
	{}

	void add_edge(int v, int u)
	{
		vertexes[v].add_edge(u);
	}

	void reverse()
	{
		Vertex *v = new Vertex[n];
		for(int i = 0; i < n; i ++)
		{
			v[i].path[0] = vertexes[i].path[0];
			v[i].path[1] = vertexes[i].path[1];
			for(int j : vertexes[i].edges())
				v[j].add_edge(i);

		}

		p = 1 - p;
		delete [] vertexes;
		vertexes = v;
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

	void calc_paths()
	{
		for(int i = 0; i < n; i++)
		{
			if(vertexes[i].path[p] == -1)
				_calc_path(vertexes[i]);
		}
	}

	void print_paths()
	{
		for(int i = 0; i < n; i++)
			cout<< vertexes[i].path[0] + vertexes[i].path[1] - 1<< " ";
		cout<< "\n";
	}

	~Graph()
	{
		delete [] vertexes;
	}
};

int main()
{
	int tasks;
	cin>> tasks;

	while(tasks--)
	{
		int n;
		int m;
		cin>> n>> m;
		Graph g(n);
		for(int i = 0; i < m; i++)
		{
			int a;
			int b;
			cin>> a>> b;
			g.add_edge(a - 1, b - 1);
		}

		g.calc_paths();
		g.reverse();
		g.calc_paths();
		g.print_paths();

	}
	return 0;
}