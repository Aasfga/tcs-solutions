#include <iostream>
#include <vector>
#include <stack>
#include <list>


struct vertex
{

	std::vector<int> _edges[2];
	int *rev = nullptr;
	int c_id = -1;
	int colour = 0;


	void add_in_edge(int v)
	{
		_edges[1].push_back(v);
	}

	void add_out_edge(int v)
	{
		_edges[0].push_back(v);
	}

	std::vector<int>& edges()
	{
		return _edges[*rev];
	}

	bool is_black()
	{
		return colour == 1;
	}

	void set_black()
	{
		colour = 1;
	}

	void set_white()
	{
		colour = 0;
	}
};


class CityMap
{
	int size;
	vertex *vertexes;
	int rev = 0;

	void postorder(int i_v, std::list<int> &order)
	{
		vertex &v = vertexes[i_v];
		if(v.is_black())
			return;
		v.set_black();

		for(int i : v._edges[rev])
			postorder(i, order);
		order.push_front(i_v);
	}

	std::list<int> postorder()
	{
		std::list<int> order;

		for(int i = 0; i < size; i++)
			vertexes[i].set_white();

		for(int i = 0; i < size; i++)
			postorder(i, order);

		return std::move(order);
	}

	void reverse()
	{
		rev = 1 - rev;
	}

	void _set_connection(int i_v, int id)
	{
		vertex &v = vertexes[i_v];
		if(v.is_black())
			return;
		v.set_black();
		v.c_id = id;
		for(int i : v._edges[rev])
			_set_connection(i, id);
	}

	void _set_connection(std::list<int> &list)
	{
		for(int i = 0; i < size; i++)
			vertexes[i].set_white();

		int id = 0;
		for(auto i : list)
		{
			if(!vertexes[i].is_black())
				_set_connection(i, id++);
		}
	}

public:

	explicit CityMap(int size) : size(size), vertexes(new vertex[size])
	{
		for(int i = 0; i < size; i++)
			vertexes[i].rev = &rev;
	}

	void add_edge(int out, int in)
	{
		vertexes[out].add_out_edge(in);
		vertexes[in].add_in_edge(out);
	}

	void set_connection()
	{
		std::list<int> order = postorder();
		for(int i : order)
			std::cout<< i<< " ";
		std::cout<< "\n";
		reverse();
		_set_connection(order);
		reverse();
	}

	bool is_connected(int v, int u)
	{
		return vertexes[v].c_id == vertexes[u].c_id;
	}

	~CityMap()
	{
		delete[] vertexes;
	}
};


int main()
{
	std::ios_base::sync_with_stdio(false);
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int size, roads, questions;
		int a, b;

		std::cin>>size>>roads;
		CityMap map(size);

		while(roads--)
		{
			std::cin>> a>> b;
			map.add_edge(a - 1, b - 1);
		}
		map.set_connection();
		std::cin>> questions;

		while(questions--)
		{
			std::cin>> a>> b;
			std::cout<< (map.is_connected(a - 1, b - 1) ? "TAK\n" : "NIE\n");
		}
	}

	return 0;
}