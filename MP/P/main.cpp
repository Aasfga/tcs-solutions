#include <iostream>
#include <vector>
#include <list>

using namespace std;


class Vertex;

class Edge
{
public:
	bool visited = false;
	int first;
	int second;
	int length;
	int sand;

	Edge()
	{}

	Edge(int first, int second, int length, int sand);

	int getVertex(int v);

	int getVertex(Edge e);
};

class Vertex
{
public:
	vector<int> edges;


	void addEdge(int index);

	int getEdge(vector<Edge> &graphEdges);

};

class Graph
{
public:
	vector<Vertex> vertexes;
	vector<Edge> edges;
	int i = 1;
	int size;

	void pGetCycle(int target, int v, int e, vector<int> &l, bool firstTime);

	Graph(int size);


	void getCycle(vector<int> &l);

	void addEdge(int x, int y, int l, int s);

	void printRoute();
};


void Vertex::addEdge(int index)
{
	edges.push_back(index);
}

int Vertex::getEdge(vector<Edge> &graphEdges)
{
	for(int i = 0; i < 4; i++)
	{
		if(!graphEdges[edges[i]].visited)
			return edges[i];
	}

	return -1;
}

int Edge::getVertex(Edge e)
{
	if(first == e.first || first == e.second)
		return first;
	else
		return second;
}

int Edge::getVertex(int v)
{
	return v == first ? second : first;
}

Edge::Edge(int first, int second, int length, int sand) : first(first), second(second), length(length), sand(sand)
{}

Graph::Graph(int size) : vertexes(size + 1), size(size + 1), edges( 2 * size + 2)
{}

void Graph::addEdge(int x, int y, int l, int s)
{
	Edge edge(x, y, l, s);
	edges[i] = edge;
	vertexes[x].addEdge(i);
	vertexes[y].addEdge(i);
	i++;
}

void Graph::getCycle(vector<int> &l)
{
	pGetCycle(1, 1, -1, l, true);
	l.pop_back();
}

void Graph::pGetCycle(int target, int v, int e, vector<int> &l, bool firstTime)
{
	if(v == target && !firstTime)
	{

	}
	else
	{
		int firstEdge = vertexes[v].getEdge(edges);
		edges[firstEdge].visited = true;
		pGetCycle(target, edges[firstEdge].getVertex(v), firstEdge, l, false);

		for(int i : vertexes[v].edges)
		{
			if(!edges[i].visited)
			{
				edges[i].visited = true;
				pGetCycle(v, edges[i].getVertex(v), i, l, false);
			}
		}
	}
	l.push_back(e);
}

void Graph::printRoute()
{
	vector<int> l;
	getCycle(l);
	int min = 100000000;
	int minI = 1;
	int sand = 0;

//	for(int x : l )
//		cout<< x<< " ";
//	cout<< endl;

	vector<int>::iterator iter = l.begin();
	do
	{
		sand += edges[*iter].sand;
		sand -= edges[*iter].length/2;

		iter++;
		if(iter == l.end())
			iter = l.begin();
		sand -= edges[*iter].length/2;

		if(sand < min)
		{
			min = sand;
			minI = *iter;
		}
	} while(*iter != *l.begin());


	cout<< l.size()<< "\n";
	iter = l.begin();
	while(*iter != minI)
		iter++;


	cout<< *iter<< " ";
	int a = *iter;
	iter++;
	if(iter == l.end())
		iter = l.begin();
	int b = *iter;

	cout<< edges[a].getVertex(edges[b])<< "\n";
	do
	{
		cout<< *iter<< "\n";
		iter++;
		if(iter == l.end())
			iter = l.begin();
	}while(*iter != minI);

}

int main()
{
	ios_base::sync_with_stdio(false);

	int sets;
	cin>> sets;

	while(sets--)
	{
		int size;
		cin>> size;
		int allLength = 0;
		int allSand = 0;
		Graph graph(size);
		for(int i = 0; i < 2 * size; i++)
		{
			int x, y, length, sand;
			cin >> x >> y >> length >> sand;
			allLength += length;
			allSand += sand;

			graph.addEdge(x, y, length, sand);
		}

		if(allSand < allLength)
			cout<< "NIE\n";
		else
		{
			cout<< "TAK\n";
			graph.printRoute();
		}

	}

	return 0;
}