#include <iostream>
#include <vector>
#include <queue>

typedef std::pair<long long, int> pair;

struct edge
{
	int vertex = 0;
	int cost = 0;
	int start = 0;
	int interval = 0;
	int length = 0;

	edge() = default;

	edge(int v, int c, int i, int s, int l) : vertex(v), start(s), interval(i), length(l), cost(c)
	{}
};

struct vertex
{
	std::vector<edge> edges;
	bool visited = false;
};




long long wait(long long time, int i, int s, int l)
{
	if(i == 0)
		return 0;
	time += i;
	time -= s;
	time %= i;
	if(time < l)
		return 0;
	else
		return i - time;


}

class Graph
{
	vertex *vertexes;
	int size;

public:

	Graph(int size) : vertexes(new vertex[size]), size(size)
	{}

	void add_edge(int u, int v, int c, int i, int s, int l)
	{
		vertexes[u].edges.emplace_back(v, c, i, s, l);
	}

	long long go_to_camp()
	{
		long long crr_time;
		int crr_v;
		std::priority_queue<pair, std::vector<pair>, std::greater<pair>> queue;
		queue.emplace(0, 0);

		while(!queue.empty())
		{
			pair p = queue.top();
			queue.pop();
			crr_time = p.first;
			crr_v = p.second;
			if(vertexes[crr_v].visited)
				continue;
			if(crr_v == size - 1)
				return crr_time;
			vertexes[crr_v].visited = true;

			for(auto e : vertexes[crr_v].edges)
			{
				long long new_time = crr_time + e.cost + wait(crr_time, e.interval, e.start, e.length);
				queue.emplace(new_time, e.vertex);
			}
		}

		return -1;
	}


	~Graph()
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
		int v_size, e_size;
		std::cin>> v_size>> e_size;
		Graph graph(v_size);

		while(e_size--)
		{
			int u, v, c, i, s, l;
			std::cin>> u>> v>> c>> i>> s>> l;
			graph.add_edge(u - 1 , v - 1, c, i, s, l);
		}

		long long cost = graph.go_to_camp();

		if(cost < 0)
			std::cout<< "NIE\n";
		else
			std::cout<< cost<< "\n";
	}
	return 0;
}