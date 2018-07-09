#include <iostream>
#include <algorithm>

struct vertex
{
	int father = 0;
	int rank = 0;

	vertex() = default;

	vertex(int f) : father(f)
	{}
};

struct edge
{
	int v;
	int u;
	int cost;

	edge(int v, int u, int cost) : v(v), u(u), cost(cost)
	{}

	edge() = default;
};

int get_father(int id, vertex *vertexes)
{
	if(vertexes[id].father == id)
		return id;
	vertexes[id].father = get_father(vertexes[id].father, vertexes);
	return vertexes[id].father;
}

void concat(int u, int v, vertex *vertexes)
{
	u = get_father(u, vertexes);
	v = get_father(v, vertexes);

	if(vertexes[u].rank < vertexes[v].rank)
		vertexes[u].father = v;
	else if(vertexes[u].rank == vertexes[v].rank)
	{
		vertexes[u].rank++;
		vertexes[v].father = u;
	}
	else
		vertexes[v].father = u;
}

int cmp(const edge &a, const edge &b)
{
	return a.cost < b.cost;
}

long long calc_cost(vertex *vertexes, edge *edges, int e_size)
{
	std::sort(edges, edges + e_size, cmp);
	long long cost = 0;
	for(int i = 0; i < e_size; i++)
	{
		edge &e = edges[i];
		if(get_father(e.v, vertexes) != get_father(e.u, vertexes))
		{
			concat(e.v, e.u, vertexes);
			cost += e.cost;
		}
	}

	return cost;
}

int main()
{
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int v_size, e_size;
		std::cin>>v_size>>e_size;
		auto *vertexes = new vertex[v_size];
		auto *edges = new edge[e_size];

		for(int i = 0; i < v_size; i++)
			vertexes[i] = vertex(i);

		for(int i = 0; i < e_size; i++)
		{
			int a, b, c;
			std::cin>>a>>b>>c;
			edges[i] = edge(a - 1, b - 1, c);
		}

		std::cout<<calc_cost(vertexes, edges, e_size)<<"\n";
		delete[] vertexes;
		delete[] edges;
	}
	return 0;
}