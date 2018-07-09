//
// Created by Dominik Grybos on 11.03.2018.
//

#include <vector>
#include <iostream>
#include <queue>


struct vertex;

typedef std::vector<vertex*> vector;


#define INIT 0
#define GREY 1
#define BROKEN 2
#define FIRST 4
#define SECOND 8
#define COVER 16
#define N_COVER 32

struct vertex
{
	int y;
	int x;
	vector adj_list;
	int state = INIT;
	vertex *mate = NULL;

	void add_edge(vertex *v)
	{
		adj_list.push_back(v);
	}

	void set_coords(int y, int x)
	{
		this->y = y;
		this->x = x;
	}

	void add_opt(int opt)
	{
		state |= opt;
	}

	void del_opt(int opt)
	{
		state &= ~opt;
	}

	bool is_opt(int opt)
	{
		return (state & opt) != 0;
	}
};

class Room
{
	vertex **vertexes;
	vector group[2];
	int height;
	int width;


	bool is_correct(int y, int x)
	{
		return 0 <= y && y < height && 0 <= x && x < width;
	}

	bool not_broken(int y, int x, std::string *lines)
	{
		return lines[y][x] != 'x';
	}

	void add_edges(int y, int x, std::string *lines)
	{
		int delta_x[] = {-1, 1};
		int delta_y[] = {-1, 0, 1};

		for(int dy : delta_y)
		{
			for(int dx : delta_x)
			{
				if(is_correct(y + dy, x + dx) &&
				   not_broken(y + dy, x + dx, lines))
					vertexes[y][x].add_edge(&vertexes[y + dy][x + dx]);
			}
		}
	}

	void create_graph(std::string *lines)
	{
		vertexes = new vertex *[height];

		for(int i = 0; i < height; i++)
			vertexes[i] = new vertex[width];
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				vertexes[y][x].set_coords(y, x);
				if(lines[y][x] == 'x')
					vertexes[y][x].add_opt(BROKEN);
				else
				{
					add_edges(y, x, lines);
					group[x % 2].push_back(&vertexes[y][x]);
					vertexes[y][x].add_opt(x % 2 ? SECOND : FIRST);
				}
			}
		}
	}

	bool create_matching(vertex *v)
	{
		if(v == nullptr)
			return true;
		if(v->is_opt(GREY))
			return false;
		v->add_opt(GREY);
		bool result = false;
		for(auto u : v->adj_list)
		{
			if(create_matching(u->mate))
			{
				v->mate = u;
				u->mate = v;
				result = true;
				break;
			}
		}

		return result;
	}

	void set_white()
	{
		for(auto v : group[0])
			v->del_opt(GREY);
		for(auto v : group[1])
			v->del_opt(GREY);

	}

	void create_matching()
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

		}while(result);

//		for(auto v : group[0])
//		{
//			if(v->mate != nullptr)
//				printf("(%d, %d) -> (%d, %d)\n", v->y, v->x, v->mate->y, v->mate->x);
//		}
	}

	void create_cover(vertex *v)
	{
		if(v->is_opt(GREY))
			return;
		v->add_opt(GREY);

		if(v->is_opt(FIRST))
		{
			v->add_opt(N_COVER);
			for(auto u : v->adj_list)
			{
				create_cover(u);
			}
		}
		else
		{
			v->add_opt(COVER);
			create_cover(v->mate);
		}
	}

	void create_cover()
	{
		set_white();
		for(auto v : group[0])
		{
			if(v->mate == nullptr)
				create_cover(v);
		}

		for(auto v :group[0])
		{
			if(!v->is_opt(N_COVER))
				v->add_opt(COVER);
		}
		for(auto v : group[1])
		{
			if(!v->is_opt(COVER))
				v->add_opt(N_COVER);
		}
	}

public:
	Room(int height, int width, std::string *lines) : height(height), width(width)
	{
		create_graph(lines);
		create_matching();
		create_cover();
	}

	void print()
	{
		int students = 0;
		std::string result;

		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				vertex &v = vertexes[y][x];
				if(v.is_opt(BROKEN))
					result += 'x';
				else if(v.is_opt(COVER))
					result += '.';
				else if(v.is_opt(N_COVER))
					result += 's';
				students += v.is_opt(N_COVER) ? 1 : 0;
			}
			result += '\n';
		}

		std::cout<< students << "\n"<< result;
	}
};

int main()
{
	std::ios_base::sync_with_stdio(false);
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int height, width;
		std::cin>>height>>width;
		auto *lines = new std::string[height];
		for(int i = 0; i < height; i++)
			std::cin>>lines[i];
		Room room(height, width, lines);
		room.print();
		delete[] lines;
	}
}

