#include <iostream>
#include <queue>


using namespace std;

class Edge
{
	int vertex;
	bool secured;

public:

	Edge(int vertex, bool secured)
	{
		this -> vertex = vertex;
		this -> secured = secured;
	}

	bool isSecured()
	{
		return secured;
	}

	int getVertex()
	{
		return vertex;
	}

	void print(string prefix)
	{
		cout<< prefix<< vertex<< "  "<< (secured ? "#\n" : "-\n");
	}
};

class Vertex
{
public:

	vector<Edge> edges;
	bool visited = false;
	int distance = -1;

	void setVisited()
	{
		visited = true;
	}

	bool isVisited()
	{
		return visited;
	}

	void setDistance(int dist)
	{
		if(!visited || distance > dist)
			distance = dist;
	}

	void addEdge(int x, bool secured)
	{
		Edge edge(x, secured);
		edges.push_back(edge);
	}

	void print()
	{
		cout<< visited<< "  "<< distance<< "\n";
		for(Edge e : edges)
			e.print("\t");
	}
};


class Graph
{
	vector<Vertex> vertexes;
public:

	Graph()
	{
		Vertex vertex;
		vertexes.push_back(vertex);
	}

	void addVertex(int n)
	{
		for(int i = 0; i < n; i++)
		{
			Vertex vertex;
			vertexes.push_back(vertex);
		}
	}

	void addEdge(int x, int y, bool secured)
	{
		vertexes[x].addEdge(y, secured);
		vertexes[y].addEdge(x, secured);
	}

	void print()
	{
		for(int i = 1; i < vertexes.size(); i++)
		{
		//	cout<< i<< " ";
			if(vertexes[i].isVisited())
				cout<< vertexes[i].distance<< "\n";
			else
				cout<< "!\n";
		}
	}

	void setDistances()
	{
		queue<int> queues[2];
		int p = 0;
		queues[p].push(1);
		queues[p].push(-1);
		int distance = 0;
		while(queues[p].size() != 1 || queues[1 - p].size() != 0)
		{
			int x = queues[p].front();
			queues[p].pop();

			if(x != -1 && !vertexes[x].isVisited())
			{
				vertexes[x].setDistance(distance);
				vertexes[x].setVisited();

				for(Edge edge : vertexes[x].edges)
				{
					int y = edge.getVertex();
					if(edge.isSecured() && !vertexes[y].isVisited())
						queues[p].push(y);
					else if(!vertexes[y].isVisited())
						queues[1 - p].push(y);
				}
			}

			if(x == -1)
			{
				distance++;
				p = 1 - p;
				queues[p].push(-1);
			}
		}
	}

};

Graph constructGraph()
{
	int size;
	int edges;
	cin>> size>> edges;
	Graph graph;
	graph.addVertex(size);
	for(int i = 0; i < edges; i++)
	{
		int x;
		int y;
		char secured;
		cin>> x>> y>> secured;
		graph.addEdge(x, y, secured == '#');
	}


	return graph;
}

int main()
{
	int sets;
	cin>> sets;

	while(sets--)
	{
		Graph graph = constructGraph();
		graph.setDistances();
		graph.print();
	}

	return 0;
}