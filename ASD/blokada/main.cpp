#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
#include <list>


class Graph
{
	struct vertex;
	struct edge;

	struct vertex
	{


		std::vector<edge *> edges;
		int id;

		vertex() = default;

	};

	struct edge
	{
		int flow;
		edge *mate;
		vertex *v;

		static void bind(edge *a, edge *b)
		{
			a->mate = b;
			b->mate = a;
		}

		edge(vertex *v, int flow) : flow(flow), v(v)
		{}

		edge() = default;

		void inc_flow(int x)
		{
			flow -= x;
			mate->flow += x;
		}
	};

	int e_size;
	int v_size;
	vertex *vertexes;
	edge *edges;
	bool *visited;
	edge **paths;
	vertex *source;
	vertex *sink;
	int e_iter = 0;


	void find_path()
	{
		std::queue<vertex*> queue;
		queue.push(source);
		visited[source->id] = true;

		while(!queue.empty())
		{
			vertex *crr = queue.front();
			queue.pop();

			if(crr == sink)
				break;
			for(auto *e : crr->edges)
			{
				vertex *mate = e->v;
				if(visited[mate->id] || e->flow == 0)
					continue;
				paths[mate->id] = e;
				visited[mate->id] = true;
				queue.push(mate);
			}
		}
	}

	void add_to_path(int value)
	{
		edge *crr = paths[sink->id];
		while(crr != nullptr)
		{
			crr->inc_flow(value);
			crr = paths[crr->mate->v->id];
		}
	}

	bool create_path()
	{
		std::fill_n(paths, v_size, nullptr);
		std::fill_n(visited, v_size, false);
		find_path();
		if(!visited[sink->id])
			return false;
		add_to_path(1);
		return true;
	}

	void _add_edge(vertex *u, vertex *v, int flow)
	{
		edge *e = edges + e_iter++;
		edge *f = edges + e_iter++;
		*e = edge(v, flow);
		*f = edge(u, 0);
		edge::bind(e, f);
		u->edges.push_back(e);
		v->edges.push_back(f);
	}

	void create_dummy_edges()
	{
		for(int i = 0; 2 * i < v_size; i++)
			_add_edge(vertexes + 2 * i, vertexes + 2 * i + 1, 1);
	}

	void set_ends(int s, int t)
	{
		source = vertexes + 2 * s;
		sink = vertexes + 2 * t + 1;
		source->edges.front()->flow = INT32_MAX;
		vertexes[2*t].edges.front()->flow = INT32_MAX;
	}

	void set_id()
	{
		for(int i = 0; i < v_size; i++)
			vertexes[i].id = i;
	}

	void set_cut(vertex *v, std::list<int> &list)
	{
		if(visited[v->id])
			return;
		visited[v->id] = true;
		list.push_back(v->id);
		for(auto e : v->edges)
		{
			if(e->flow != 0)
				set_cut(e->v, list);
		}
	}

	void set_cut(std::list<int> &list)
	{
		std::fill_n(visited, v_size, false);
		set_cut(source, list);
	}

	void get_blockade(std::list<int> &list, std::list<int> &result)
	{
		for(auto x : list)
		{
			if(x % 2 == 0 && !visited[x + 1])
				result.push_back(x / 2 + 1);
		}
	}

public:

	Graph(int v_s, int e_s, int source, int sink) : v_size(2 * v_s), e_size(2 * v_s + 4 * e_s)
	{
		vertexes = new vertex[v_size];
		edges = new edge[e_size];
		visited = new bool[v_size];
		paths = new edge *[v_size];
		create_dummy_edges();
		set_id();
		set_ends(source - 1 , sink - 1);
	}

	void add_edge(int u, int v)
	{
		_add_edge(vertexes + 2 * u - 1, vertexes + 2 * v - 2, INT32_MAX);
		_add_edge(vertexes + 2 * v - 1, vertexes + 2 * u - 2, INT32_MAX);
	}


	void set_flow()
	{
		while(create_path());
	}

	int get_flow()
	{
		return source->edges.front()->mate->flow;
	}

	void get_blockade(std::list<int> &result)
	{
		std::list<int> list;
		set_cut(list);
		get_blockade(list, result);
	}

	~Graph()
	{
		delete [] visited;
		delete [] paths;
		delete [] edges;
		delete [] vertexes;
	}
};


int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int n, m, s, t;
		std::cin>> n>> m>> s>> t;
		Graph graph(n, m, s, t);
		while(m--)
		{
			std::cin>> s>> t;
			graph.add_edge(s, t);
		}
		graph.set_flow();
		std::list<int> blockade;
		graph.get_blockade(blockade);
		std::cout<< blockade.size()<< "\n";
		for(auto x : blockade)
			std::cout<< x<< " ";
		std::cout<< "\n";
	}
}
