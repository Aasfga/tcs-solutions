#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class Tree
{
	class Vertex
	{
	public:
		vector<int> children;
		long long opt_r = -1;
		long long opt_n = -1;
		long long cost = 0;
		char police = ' ';

		Vertex(int c)
		{
			cost = c;
		}

		bool has_opt_set()
		{
			return opt_r != -1;
		}

		void add_edge(int v)
		{
			children.push_back(v);
		}
	};


	vector<Vertex> vertexes;


	void set_costs(int c, int p)
	{
		Vertex &v = vertexes[c];
		v.opt_r = v.cost;
		v.opt_n = 0;
		if(v.children.empty() || (p != -1 && v.children.size() <= 1))
		{
			v.opt_n = 0;
			return;
		}

		for(int i : v.children)
		{
			if(i == p)
				continue;
			Vertex &u = vertexes[i];
			if(!u.has_opt_set())
				set_costs(i, c);
			v.opt_n += u.opt_r;
			v.opt_r += min(u.opt_r, u.opt_n);
		}
	}

	void set_solution(int c, int opt, int p)
	{
		Vertex &v = vertexes[c];
		if(opt == 0)
		{
			v.police = '1';
			for(int i : v.children)
			{
				if(i == p)
					continue;
				set_solution(i, 1, c);
			}
		}
		else
		{
			v.police = (v.opt_r <= v.opt_n ? '1' : '0');
			for(int i : v.children)
			{
				if(i == p)
					continue;
				set_solution(i, v.opt_r <= v.opt_n, c);
			}
		}
	}


public:

	void add_edge(int u, int v)
	{
		vertexes[u].add_edge(v);
		vertexes[v].add_edge(u);
	}

	void add_vertex(long long cost)
	{
		vertexes.emplace_back(cost);
	}

	void set_costs()
	{
		if(!vertexes.empty())
			set_costs(0, -1);
	}

	void set_solution()
	{
		cout<< min(vertexes[0].opt_r, vertexes[0].opt_n)<< "\n";
		set_solution(0, vertexes[0].opt_r > vertexes[0].opt_n, -1);

	}

	void print_solution()
	{
		for(Vertex &v: vertexes)
			cout<< v.police;
		cout<< "\n";
	}
};


int main()
{
	int task;
	cin>> task;

	while(task--)
	{
		Tree tree;
		int n;
		cin>> n;
		for(int i = 0; i < n; i++)
		{
			int c;
			cin>> c;
			tree.add_vertex(c);
		}

		for(int i = 1; i < n; i++)
		{
			int a, b;
			cin>> a>> b;
			tree.add_edge(a - 1, b - 1);
		}

		tree.set_costs();
		tree.set_solution();
		tree.print_solution();
	}

	return 0;
}