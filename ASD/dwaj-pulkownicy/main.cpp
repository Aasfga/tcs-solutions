#include <vector>
#include <iostream>
#include <algorithm>


#define INIT        0
#define GREY        1
#define MATCHED     2
#define GROUP       4
#define SWAMP       8
#define KMICIC      16

class Graph
{
	struct vertex
	{
		int crr_edge = 0;
		int opts = INIT;
		vertex *edges[5];
		vertex *mate = nullptr;

		static void bind(vertex *v, vertex *u)
		{
			v->mate = u;
			u->mate = v;
			v->add_opt(MATCHED);
			u->add_opt(MATCHED);
		}

		vertex()
		{
			std::fill_n(edges, 5, nullptr);
		}

		void add_edge(vertex *v)
		{
			edges[crr_edge++] = v;
		}

		void add_opt(int opt)
		{
			opts |= opt;
		}

		void del_opt(int opt)
		{
			opts &= ~opt;
		}

		bool is_opt(int opt)
		{
			return (opts & opt) != 0;
		}

		void set_group(int g_id)
		{
			add_opt(g_id * GROUP);
		}

		int get_group()
		{
			if(is_opt(GROUP))
				return 1;
			return 0;
		}

	};

	int width;
	int height;
	vertex **vertexes;
	std::vector<vertex *> group[2];

	bool is_correct(int y, int x)
	{
		return 0 <= y && y < height && 0 <= x && x < width;
	}

	bool is_swamp(int y, int x)
	{
		return vertexes[y][x].is_opt(SWAMP);
	}

	void set_swamps(std::string *lines)
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				if(lines[y][x] == 'X')
					vertexes[y][x].add_opt(SWAMP);
			}
		}
	}

	void set_edges(int y, int x)
	{
		int array[] = {0, 1, 0, -1, 0};

		for(int i = 0; i < 4; i++)
		{
			int n_y = y + array[i + 1];
			int n_x = x + array[i];
			if(is_correct(n_y, n_x) && !is_swamp(n_y, n_x))
				vertexes[y][x].add_edge(&vertexes[n_y][n_x]);
		}
	}

	void set_graph()
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				if(!vertexes[y][x].is_opt(SWAMP))
				{
					set_edges(y, x);
					group[(y + x) % 2].push_back(&vertexes[y][x]);
				}
			}
		}
	}

	void set_white()
	{
		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
				vertexes[y][x].del_opt(GREY);
		}
	}

	bool create_matching(vertex *v)
	{
		if(v == nullptr)
			return true;
		if(v->is_opt(GREY))
			return false;
		v->add_opt(GREY);
		for(int i = 0; v->edges[i] != nullptr; i++)
		{
			vertex *u = v->edges[i];
			if(create_matching(u->mate))
			{
				vertex::bind(v, u);
				return true;
			}
		}
		return false;
	}


	void starts_even(vertex *v)
	{
		if(v == nullptr)
			return;
		if(v->is_opt(GREY))
			return;
		v->add_opt(GREY | KMICIC);
		for(int i = 0; v->edges[i] != nullptr; i++)
			starts_odd(v->edges[i]);
	}

	void starts_odd(vertex *v)
	{
		if(v == nullptr)
			return;
		if(v->is_opt(GREY))
			return;
		v->add_opt(GREY);
		starts_even(v->mate);
	}

public:
	Graph(int height, int width, std::string *lines) : width(width), height(height)
	{
		vertexes = new vertex *[height];
		for(int i = 0; i < height; i++)
			vertexes[i] = new vertex[width];
		set_swamps(lines);
		set_graph();
	}


	void print_matched()
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; ++x)
			{
				if(vertexes[y][x].is_opt(SWAMP))
					std::cout<< "x";
				else if(vertexes[y][x].is_opt(MATCHED))
					std::cout<< "m";
				else
					std::cout<< ".";
			}
			std::cout<< "\n";
		}
	}

	void print()
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; ++x)
			{
				if(vertexes[y][x].is_opt(SWAMP))
					std::cout<< "X";
				else if(vertexes[y][x].is_opt(KMICIC))
					std::cout<< "K";
				else
					std::cout<< "W";
			}
			std::cout<< "\n";
		}

	}

	void matching()
	{
		bool result;
		do
		{
			result = false;
			for(auto v : group[0])
			{
				if(v->mate == nullptr && create_matching(v))
					result = true;
			}
			set_white();

		} while(result);
	}

	void set_starts()
	{
		set_white();
		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				if(!vertexes[y][x].is_opt(MATCHED | SWAMP))
					starts_even(&vertexes[y][x]);
			}
		}
	}

	~Graph()
	{
		for(int i = 0; i < height; i++)
			delete[] vertexes[i];
		delete[] vertexes;
	}
};

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int height, width;
		std::cin>> height>> width;
		std::string *lines = new std::string[height];
		for(int i = 0; i < height; i++)
			std::cin>> lines[i];
		Graph graph(height, width, lines);
		graph.matching();
//		graph.print_matched();
		graph.set_starts();
		graph.print();
		delete [] lines;
	}
}