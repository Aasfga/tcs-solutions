#include <vector>
#include <iostream>
#include <queue>
#include <list>

using namespace std;

enum Group
{
	FIRST = 0,
	SECOND = 1
};


class Vertex
{
	friend class Graph;

	vector<int> neighbors;
	char group = -1;
	unsigned int time = 0;
	bool visited = false;


public:

	Vertex()
	{
		time = 0;
	}

	Vertex(int time) : time(time)
	{}

	void addEdge(int v)
	{
		neighbors.push_back(v);
	}

	bool isVisited()
	{
		return visited;
	}

	void setVisited()
	{
		visited = true;
	}

	bool isInGroup()
	{
		return group != -1;
	}

	void setGroup(Group g)
	{
		group = g;
	}

	void setGroup(Vertex &v)
	{
		group = 1 - v.group;
	}
};

class Graph
{
	int size;
	Vertex *vertexes;
	unsigned int maxTime = 0;

	int getNotVisitedVertex(int last)
	{
		while(last < size && vertexes[last].isVisited())
			last++;

		return last;
	}

public:

	Graph(int size) : vertexes(new Vertex[size + 1]), size(size + 1)
	{
	}

	bool setGroups()
	{
		int last = 1;
		queue<int, list<int>> q;


		while(last < size || !q.empty())
		{
			if(q.empty())
			{
				last = getNotVisitedVertex(last);
				if(last >= size)
					break;
				else
				{
					vertexes[last].setVisited();
					q.push(last);
				}
			}

			int crr = q.front();
			if(crr <= 0 || crr > size)
				cout<< "LOLOLO";
			Vertex *v = &vertexes[q.front()];
			q.pop();


			if(!v -> isInGroup())
				v -> setGroup(FIRST);

			maxTime = max(maxTime, v -> time);
			for(int x : v -> neighbors)
			{
				if(x <= 0 || x > size)
					cout<< "LOLOLO";
				if(vertexes[x].group == v -> group)
					return false;

				maxTime = (unsigned int) max((int)maxTime, (int) (v -> time + vertexes[x].time));
				vertexes[x].setGroup(*v);
				if(!vertexes[x].isVisited())
				{
					vertexes[x].setVisited();
					q.push(x);
				}
			}
		}

		return true;
	}

	void addVertex(int time)
	{
		Vertex v(time);
		//vertexes.push_back(v);
	}

	void addEdge(int x, int y)
	{
		vertexes[x].addEdge(y);
		vertexes[y].addEdge(x);
	}

	void print()
	{
		cout<< maxTime<< "\n";
		for(int i = 1; i < size; i++)
		{
			if(vertexes[i].group == 0)
				cout<< "0 "<< vertexes[i].time<< "\n";
			else
				cout<< maxTime - vertexes[i].time<< " "<< maxTime<< "\n";
		}
	}

	void printWithSiblings()
	{
		cout<< maxTime<< "\n";
		for(int i = 1; i < size; i++)
		{
			cout<< i<< " group: "<< vertexes[i].group<< " time: "<< vertexes[i].time<< "\n";
			for(int x : vertexes[i].neighbors)
				cout<< x<< " ";
			cout<< "\n";
		}
	}

	void addVertex(int i, int time)
	{
		Vertex v(time);
		if(i >= size)
			cout<< "LOLOLOLO";
		vertexes[i] = v;
	}

	~Graph()
	{
		delete [] vertexes;
	}
};

void constructGraph(int size, int tasks, Graph &graph)
{

	for(int i = 0; i < size; i++)
	{
		int time;
		cin>> time;

		graph.addVertex(i + 1, time);
	}

	for(int i = 0; i < tasks; i++)
	{
		int x;
		int y;
		cin>> x>> y;

		graph.addEdge(x, y);
	}


}

int main()
{
	ios_base::sync_with_stdio(false);

	int sets;
	cin>> sets;

	while(sets--)
	{
		int size, tasks;
		cin>> size>> tasks;
		Graph graph(size);
		constructGraph(size, tasks, graph);

		if(graph.setGroups())
		{
			graph.print();
		}
		else
			cout<< "ERROR\n";
	}
	return 0;
}
