#include <iostream>
#include <vector>
#include <queue>

typedef std::pair<int, int> my_pair;

struct vertex
{
	std::vector<my_pair> edges;
	bool visited = false;
};

class PrisonRunner
{
	vertex *vertexes;
	int size;


public:

	explicit PrisonRunner(int size) : vertexes(new vertex[size]), size(size)
	{}

	void add_corridor(int a, int b, int c)
	{
		vertexes[a].edges.emplace_back(b, c);
	}

	int freedom_distance()
	{
		using namespace std;
		int crr_cost;
		int crr_v;
		priority_queue<my_pair, vector<my_pair>, greater<my_pair>> queue;
		queue.emplace(0, 0);

		while(!queue.empty())
		{
			my_pair p = queue.top();
			queue.pop();
			crr_cost = p.first;
			crr_v = p.second;
			if(vertexes[crr_v].visited)
				continue;
			if(crr_v == size - 1)
				return crr_cost;
			vertexes[crr_v].visited = true;

			for(auto i : vertexes[crr_v].edges)
			{
				int n_v = i.first;
				int n_cost = i.second;

				if(!vertexes[n_v].visited)
					queue.emplace(n_cost + crr_cost, n_v);
			}
		}

		return -1;
	}
};

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int size, edges;
		std::cin>> size>> edges;
		PrisonRunner g(size);

		while(edges--)
		{
			int a, b, c;
			std::cin>> a>> b>> c;
			g.add_corridor(a - 1, b - 1, c);
		}

		int run_cost = g.freedom_distance();
		if(run_cost < 0)
			std::cout<< "BRAK\n";
		else
			std::cout<< run_cost<< "\n";
	}

	return 0;
}