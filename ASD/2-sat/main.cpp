#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <set>


typedef std::set<int> set;
typedef std::list<int> list;
#define START 0

int calc_variable(int x)
{
	if(x == 1)
		return 0;
	else if(x == -1)
		return 1;

	x = x > 0 ? x - 1 : x + 1;
	x *= 2;

	return x < 0 ? -x + 1 : x;
}

int neg(int x)
{
	return x % 2 == 0 ? x + 1 : x - 1;
}

class vertex
{
public:
	std::vector<int> _edges[2];
	int group = -1;
	int colour = 0;


	void set_group(int g)
	{
		group = g;
	}

	int get_group()
	{
		return group;
	}

	void add_in_edge(int v)
	{
		_edges[1].push_back(v);
	}

	void add_out_edge(int v)
	{
		_edges[0].push_back(v);
	}
};

class SatSolver
{
	std::vector<vertex> vertexes;
	std::vector<std::vector<int>> g_vertexes;
	std::vector<int> values;
	int size;
	int rev = 0;
	int max_group = -1;

	void postorder(int i_v, std::list<int> &order)
	{
		vertex &v = vertexes[i_v];
		if(v.colour == 1)
			return;
		v.colour++;

		for(int i : v._edges[rev])
			postorder(i, order);
		order.push_front(i_v);
	}

	void postorder(std::list<int> &order)
	{
		for(int i = START; i < size; i++)
			postorder(i, order);
	}

	void reverse()
	{
		rev = 1 - rev;
	}

	void _set_connection(int i_v, int group)
	{
		vertex &v = vertexes[i_v];
		if(v.colour == 2)
			return;
		v.colour++;
		v.set_group(group);
		for(int i : v._edges[rev])
			_set_connection(i, group);
	}

	void _set_connection(std::list<int> &list)
	{
		int id = 0;
		for(auto i : list)
		{
			if(vertexes[i].colour != 2)
				_set_connection(i, id++);
		}
		max_group = id;
	}

	void add_edge(int out, int in)
	{
		vertexes[out].add_out_edge(in);
		vertexes[in].add_in_edge(out);
	}

	void set_connection()
	{
		std::list<int> order;
		postorder(order);
		reverse();
		_set_connection(order);
		reverse();
	}

	void set_group_graph()
	{
		g_vertexes = std::vector<std::vector<int>>(max_group);
		values = std::vector<int>(max_group, -1);
		for(int i = START; i < size; i++)
		{
			int group = vertexes[i].get_group();
			g_vertexes[group].push_back(i);
		}
	}

	void set_value(int g_id)
	{
		int other_v = neg(*g_vertexes[g_id].begin());
		int other_g = vertexes[other_v].get_group();
		values[g_id] = 1;
		values[other_g] = 0;
	}

	bool is_correct()
	{
		for(int i = START; i < size; i+=2)
		{
			if(vertexes[i].get_group() == vertexes[i+1].get_group())
				return false;
		}
		return true;
	}

	bool is_set(int g_id)
	{
		return values[g_id] != -1;
	}

	int set_values()
	{

		for(int i = max_group - 1; i >= 0; i--)
		{
			if(!is_set(i))
				set_value(i);
		}

		return 0;
	}


public:

	explicit SatSolver(int size) : size(2 * size), vertexes(2 * size)
	{}

	void add_clause(int a, int b)
	{
		a = calc_variable(a);
		b = calc_variable(b);
		add_edge(neg(a), b);
		add_edge(neg(b), a);
	}

	int solve()
	{
		set_connection();
		if(!is_correct())
			return -1;
		set_group_graph();
		if(set_values() < 0)
			return -1;
		return 0;
	}

	void print_answer()
	{
		for(int i = 0; i < size; i+=2)
			std::cout<< values[vertexes[i].get_group()]<< " ";
		std::cout<< "\n";
	}
};


int main()
{
	std::ios_base::sync_with_stdio(false);
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int size, clauses;
		int a, b;
		std::cin>>size>>clauses;
		SatSolver solver(size);

		for(int i = 0; i < clauses; i++)
		{
			std::cin>>a>>b;
			solver.add_clause(a, b);
		}

		if(solver.solve() < 0)
			std::cout<<"NIE\n";
		else
		{
			std::cout<<"TAK\n";
			solver.print_answer();
		}
	}

	return 0;
}
