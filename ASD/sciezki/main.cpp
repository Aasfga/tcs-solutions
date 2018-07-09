#include <iostream>
#include <vector>

struct vertex
{
public:
	std::vector<int> children;
	long long paths = 0;
	bool has_start = false;
	bool is_terminal = false;
	bool is_set = false;

};

class Tree
{
	vertex *vertexes;
	int size;

	void _set_paths(int i)
	{
		vertex &v = vertexes[i];
		v.is_set = true;
		int starts_in_children = 0;
		for(auto c : v.children)
		{
			if(vertexes[c].is_set)
				continue;
			_set_paths(c);
			if(vertexes[c].has_start)
				starts_in_children++;
			v.paths += vertexes[c].paths;
		}
		if(starts_in_children >= 2)
			v.paths++;
		else if(starts_in_children == 1 && v.is_terminal)
			v.paths++;
		else if(starts_in_children == 1 || v.is_terminal)
			v.has_start = true;
	}

public:

	explicit Tree(int size): size(size), vertexes(new vertex[size])
	{}

	void add_edge(int a, int b)
	{
		vertexes[a].children.push_back(b);
		vertexes[b].children.push_back(a);
	}

	void add_terminal(int t)
	{
		vertexes[t].is_terminal = true;
	}

	long long calc_paths()
	{
//		int max_path = 0;
//		for(int i = 0; i < size; i++)
//		{
//			if(!vertexes[i].is_set)
//				_set_paths(i);
//			max_path = std::max(max_path, vertexes[i].paths);
//		}

		_set_paths(0);

		return vertexes[0].paths;
	}

	~Tree()
	{
		delete [] vertexes;
	}
};

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int n;
		int t;
		std::cin>> n;
		Tree tree(n);
		for(int i = 1; i < n; i++)
		{
			int a, b;
			std::cin>> a>> b;
			tree.add_edge(a-1, b-1);
		}
		std::cin>> t;
		while(t--)
		{
			int x;
			std::cin>> x;
			tree.add_terminal(x - 1);
		}

		std::cout<< tree.calc_paths()<< "\n";

	}
	return 0;
}