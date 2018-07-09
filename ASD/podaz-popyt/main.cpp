#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>



class vertex;

class edge
{
public:


	vertex *v;
	int flow;
	edge *mate;

	edge(vertex *v, int flow) : v(v), flow(flow), mate(nullptr)
	{}

public:

	edge()
	{}

	int inc_flow(int x)
	{
		if(flow - x < 0)
			return -1;
		flow -= x;
		mate->flow += x;
		return 0;
	}

	int get_flow()
	{
		return flow;
	}

	vertex *get_vertex()
	{
		return v;
	}
};

void bind(edge *f, edge *s)
{
	f->mate = s;
	s->mate = f;
}

edge *create_edge(vertex *out, vertex *in, int flow)
{
	auto *f = new edge(in, flow);
	bind(f, new edge(out, 0));
	return f;
}

class vertex
{
public:

	std::vector<edge *> edges;

	edge *parent_edge = nullptr;

};


class Graph
{
public:

	const int MAX_FLOW = INT32_MAX;

	vertex *vertexes;
	vertex *source = new vertex;
	vertex *sink = new vertex;
	int size;

	Graph()
	{}

	Graph(int size)
	{
		this->size = size;
		this->vertexes = new vertex[size];
	}

	void prepare()
	{
		for(int i = 0; i < size; i++)
			vertexes[i].parent_edge = nullptr;
		sink->parent_edge = nullptr;
		source->parent_edge = nullptr;
	}

	void find_path()
	{
		std::queue<vertex *> queue;
		queue.push(source);

		while(!queue.empty())
		{
			vertex *crr = queue.front();
			queue.pop();

			if(crr == sink)
				break;
			for(auto e : crr->edges)
			{
				vertex *mate = e->get_vertex();
				if(mate == source || mate->parent_edge != nullptr || e->flow == 0)
					continue;
				mate->parent_edge = e;
				queue.push(mate);
			}
		}
	}


	void set_list(std::vector<edge *> &list)
	{
		vertex *crr = sink;

		while(crr != source)
		{
			list.push_back(crr->parent_edge);
			crr = crr->parent_edge->mate->get_vertex();
		}
	}

	bool create_path()
	{
		prepare();
		find_path();
		if(sink->parent_edge == nullptr)
			return false;
		std::vector<edge *> list;
		set_list(list);
		int min = INT32_MAX;
		for(auto e : list)
			min = std::min(e->flow, min);
		for(auto e : list)
			e->inc_flow(min);
		return true;
	}

public:

	void set_flow()
	{
		while(create_path());
	}

	int get_flow()
	{
		int sum = 0;
		for(auto e : source->edges)
			sum += e->mate->flow;
		//TODO ^
		return sum;
	}

	~Graph()
	{
		delete [] vertexes;
		delete sink;
		delete source;
	}
};

class GraphBuilder
{
	Graph *graph;
	int size;
	vertex *vertexes;
	vertex *sink;
	vertex *source;

	void add_edge(vertex *out, vertex *in, int flow)
	{
		vertex *max_v = vertexes + size;
		if(in != sink && in >= max_v)
			return;
		if(out != source && out >= max_v)
			return;
		edge *e = create_edge(out, in, flow);
		out->edges.push_back(e);
		in->edges.push_back(e->mate);
	}

public:

	GraphBuilder() = default;

	void start_creating(int size)
	{
		graph = new Graph(size);
		this->size = size;
		this->sink = graph->sink;
		this->source = graph->source;
		this->vertexes = graph->vertexes;
	}

	void add_sink_edge(int out, int flow)
	{
		add_edge(vertexes + out, sink, flow);
	}

	void add_source_edge(int in, int flow)
	{
		add_edge(source, vertexes + in, flow);
	}

	void add_edge(int out, int in, int flow)
	{
		add_edge(vertexes + out, vertexes + in, flow);
	}

	Graph* get_graph()
	{
		return graph;
	}

	void clean()
	{
		vertexes = nullptr;
		size = 0;
		source = nullptr;
		sink = nullptr;
		graph = nullptr;
	}
};

int main()
{
//	GraphBuilder graph_builder;
//	int f = INT32_MAX / 2;
//	graph_builder.start_creating(6)
//	             .add_sink_edge(3, 5)
//	             .add_sink_edge(4, 3)
//	             .add_sink_edge(5, 2)
//	             .add_source_edge(0, 2)
//	             .add_source_edge(1, 1)
//	             .add_source_edge(2, 6)
//	             .add_edge(0, 3, 1000)
//	             .add_edge(0, 5, 1000)
//	             .add_edge(1, 3, 1000)
//	             .add_edge(1, 4, 1000)
//	             .add_edge(2, 4, 1000)
//	             .add_edge(2, 5, 1000);
//	Graph *graph = *graph_builder;
//	graph->set_flow();
//	std::cout<<graph->get_flow()<<"\n";
//	std::cout<<2 * f<<"\n";

	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int factories, shops;
		int flow;
		std::cin>> factories>> shops;
		GraphBuilder graphBuilder;
		graphBuilder.start_creating(factories + shops);
		for(int i = 0; i < factories; i++)
		{
			std::cin>> flow;
			graphBuilder.add_source_edge(i, flow);
		}

		for(int i = 0; i < shops; i++)
		{
			std::cin>> flow;
			graphBuilder.add_sink_edge(factories + i, flow);
		}

		for(int i = 0; i < factories; i++)
		{
			for(int j = 0; j < shops; j++)
			{
				std::cin>> flow;
				if(flow)
					graphBuilder.add_edge(i, factories + j, INT32_MAX);
			}
		}
		Graph *graph = graphBuilder.get_graph();
		graph->set_flow();
		std::cout<< graph->get_flow()<< "\n";
		delete graph;
	}
	return 0;
}