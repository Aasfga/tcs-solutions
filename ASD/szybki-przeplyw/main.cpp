//
// Created by Dominik Grybos on 07.04.2018.
//
#include <iostream>
#include <queue>
#include <vector>

class Graph
{
	struct vertex;
	struct edge;

	struct vertex
	{
		std::vector<edge *> edges;
		bool visited = false;
		int height = 0;
		u_long overflow = 0;
		int crr_edge = 0;

		edge *next()
		{
			return edges[crr_edge++];
		}

		edge *current()
		{
			return edges[crr_edge];
		}

		bool has_next()
		{
			return crr_edge < edges.size();
		}

		bool has_overflow()
		{
			return overflow > 0;
		}

		void increase(u_long of)
		{
			overflow += of;
		}

		void reset()
		{
			crr_edge = 0;
		}

		void set_height(int h)
		{
			height = h;
		}
	};

	struct edge
	{
		unsigned long flow = 0;
		edge *mate = nullptr;
		vertex *v = nullptr;

		static void bind(edge *a, edge *b)
		{
			a->mate = b;
			b->mate = a;
		}

		edge(vertex *v, unsigned long flow) : flow(flow), v(v)
		{}

		edge() = default;

		void inc_flow(unsigned long x)
		{
			flow -= x;
			mate->flow += x;
		}

		bool exists()
		{
			return flow > 0;
		}

		int end_height()
		{
			return v->height;
		}
	};

	vertex *source;
	vertex *sink;
	vertex *vertexes;
	edge *edges;

	int rel_counter = 0;
	int v_size;
	int e_size;
	int e_iter = 0;


	void _add_edge(vertex *u, vertex *v, unsigned long flow)
	{
		edge *e = edges + e_iter++;
		edge *f = edges + e_iter++;
		*e = edge(v, flow);
		*f = edge(u, 0);
		edge::bind(e, f);
		u->edges.push_back(e);
		v->edges.push_back(f);
	}

	void clear()
	{
		for(int i = 0; i < v_size; i++)
			vertexes[i].visited = false;
	}

	void set_heights(vertex *v)
	{
		std::queue<vertex *> queue;
		queue.push(v);
		v->visited = true;

		while(!queue.empty())
		{
			vertex *crr = queue.front();
			queue.pop();
			crr->crr_edge = 0;

			for(auto e : crr->edges)
			{
				vertex *next = e->v;
				if(next->visited || e->mate->flow == 0)
					continue;
				next->height = crr->height + 1;
				next->visited = true;
				queue.push(next);
			}
		}
	}

	void set_heights()
	{
		clear();
		source->visited = true;
		sink->visited = true;
		set_heights(sink);
		set_heights(source);
	}

	bool add_overflow(vertex *v, u_long overflow)
	{
		if(v == sink || v == source || overflow == 0)
			return false;
		bool result = !v->has_overflow();
		v->increase(overflow);
		return result;
	}

	void relabel(vertex *v)
	{
		rel_counter++;
		if(rel_counter == v_size)
		{
			set_heights();
			rel_counter = 0;
			return;
		}

		v->reset();
		int min = INT32_MAX;
		for(auto e : v->edges)
		{
			if(e->exists())
				min = std::min(min, e->end_height());
		}
		v->set_height(min + 1);
	}

	unsigned long push(edge *e, unsigned long overflow, std::queue<vertex *> &queue)
	{
		unsigned long min_flow = std::min(overflow, e->flow);
		e->inc_flow(min_flow);
		if(add_overflow(e->v, min_flow))
			queue.push(e->v);
		return min_flow;
	}

	void discharge(vertex *v, std::queue<vertex *> &queue)
	{
		while(v->overflow > 0)
		{
			for(; v->crr_edge < v->edges.size() && v->overflow != 0;)
			{
				edge *e = v->next();
				if(v->height == e->v->height + 1)
					v->overflow -= push(e, v->overflow, queue);
			}
			if(v->has_overflow())
				relabel(v);
		}
	}

	void init_flow(std::queue<vertex *> &queue)
	{
		edge *e = source->edges.front();
		e->v->overflow = e->flow;
		e->inc_flow(e->flow);
		queue.push(e->v);
	}

public:

	Graph(int v_s, int e_s, int s, int t) : v_size(v_s + 2), e_size(2 * (e_s + 2))
	{
		vertexes = new vertex[v_size];
		edges = new edge[e_size];
		source = vertexes;
		sink = vertexes + v_size - 1;
		_add_edge(source, vertexes + s, LONG_LONG_MAX);
		_add_edge(vertexes + t, sink, LONG_LONG_MAX);
		source->height = v_size - 1;

	}

	void add_edge(int u, int v, unsigned long flow)
	{
		_add_edge(vertexes + u, vertexes + v, flow);
	}

	void set_flow()
	{
		std::queue<vertex *> to_discharge;

		init_flow(to_discharge);

		while(!to_discharge.empty())
		{
			vertex *crr = to_discharge.front();
			to_discharge.pop();
			discharge(crr, to_discharge);
		}
	}

	unsigned long get_flow()
	{
		return source->edges[0]->mate->flow;
	}

	void print_heights()
	{
		for(int i = 0; i < v_size; i++)
			std::cout<<vertexes[i].height<<" ";
		std::cout<<"\n";
	}

	~Graph()
	{
		delete[] vertexes;
		delete[] edges;
	}
};

int main()
{
	std::ios_base::sync_with_stdio(false);
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int n, m, s, t, f;
		std::cin>>n>>m>>s>>t;
		Graph graph(n, m, s, t);
		while(m--)
		{
			std::cin>>s>>t>>f;
			graph.add_edge(s, t, f);
		}
		graph.set_flow();
		std::cout<<graph.get_flow()<<"\n";
	}

	return 0;
}