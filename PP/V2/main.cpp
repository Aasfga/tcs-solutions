#include <iostream>

using namespace std;

int ADD = 0;
int START = 2000000;

class point
{
	int x;
	int y;

public:

	point(int x = 0, int y = 0)
	{
		this -> x = x;
		this -> y = y;
	}

	int getX()
	{
		return x;
	}

	int getY()
	{
		return y;
	}

	void print()
	{
		cout<< x<< " "<< y<< "\n";
	}
};

class queue
{
	int capacity;
	int begin;
	int size;
	point *array;

public:


	queue(int cap = 15)
	{
		begin = 0;
		size = 0;
		capacity = cap;
		array = new point[capacity];
	}

	bool empty()
	{
		return size == 0;
	}

	bool full()
	{
		return size == capacity;
	}

	void clear()
	{
		size = 0;
		begin = 0;
	}

	void enqueue(point &x)
	{
		if(full())
			return;

		array[(begin + size) % capacity] = x;
		size++;
	}

	point dequeue()
	{
		if(empty())
		{
			point zero;
			return zero;
		}


		point ans = array[begin];
		begin++;
		begin %= capacity;
		size--;
		return ans;
	}

	point front()
	{
		if(empty())
		{
			point zero;
			return zero;
		}


		return array[begin];
	}

	void resize(int n)
	{
		if(n <= capacity)
			return;
		point* nArray = new point[n];

		for(int i = 0; i < size; i++)
		{
			nArray[i] = array[(begin + i) % capacity];
		}

		capacity = n;
		begin = 0;
		delete [] array;
		array = nArray;

	}

	~queue()
	{
		delete [] array;
	}

	void print()
	{

		for(int i = 0; i < capacity; i++)
			array[i].print();

		cout<< "Begin: " << begin << "\nSize: "<< size << "\n";
	}
};

class City
{
	int **array;
	int **distances;
	int width;
	int height;



	void defaultDistances()
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				distances[i][j] = -1;
			}
		}
	}

	void createArrays()
	{
		array = new int *[height];
		distances = new int *[height];
		for(int i = 0; i < height; i++)
		{
			distances[i] = new int[width];
			array[i] = new int[width];
		}
	}

	void readArray()
	{
		if(width == 0 || height == 0)
		{
			return;
		}

		for(int i = 0; i < height; i++)
		{
			string input;
			cin >> input;

			for(int j = 0; j < width; j++)
			{
				array[i][j] = input[j] - '0';
			}
		}
	}


	void addNetCafe(queue &aQueue)
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				if(array[i][j] == 1)
				{
					distances[i][j] = 0;
					point p(j, i);
					aQueue.enqueue(p);
				}
			}
		}
	}

	bool areCordsOk(int x, int y)
	{
		if(y < 0 || y >= height)
		{
			return false;
		}
		if(x < 0 || x >= width)
		{
			return false;
		}

		return true;
	}

	void go(queue &aQueue, int x, int y)
	{
		if(!areCordsOk(x, y))
		{
			return;
		}

		if(array[y][x] != 2 && distances[y][x] < 0)
		{
			point p(x, y);
			aQueue.enqueue(p);
		}
	}

	void updateDistances(int x, int y)
	{
		if(areCordsOk(x, y + 1) && distances[y + 1][x] < 0)
		{
			distances[y + 1][x] = 1 + distances[y][x];
		}

		if(areCordsOk(x, y - 1) && distances[y - 1][x] < 0)
		{
			distances[y - 1][x] = 1 + distances[y][x];
		}

		if(areCordsOk(x + 1, y) && distances[y][x + 1] < 0)
		{
			distances[y][x + 1] = 1 + distances[y][x];
		}

		if(areCordsOk(x - 1, y) && distances[y][x - 1] < 0)
		{
			distances[y][x - 1] = 1 + distances[y][x];
		}

	}

	void calculateDistances()
	{
		queue aQueue(START);
		addNetCafe(aQueue);

		while(!aQueue.empty())
		{
			point p = aQueue.dequeue();
			int x = p.getX();
			int y = p.getY();
			go(aQueue, x, y + 1);
			go(aQueue, x, y - 1);
			go(aQueue, x + 1, y);
			go(aQueue, x - 1, y);
			updateDistances(x, y);
		}
	}


public:

	City()
	{
		cin >> height >> width;
		createArrays();
		readArray();
		defaultDistances();
		calculateDistances();
	}


	~City()
	{
		for(int i = 0; i < height; i++)
		{
			delete[] array[i];
			delete[] distances[i];
		}

		delete[] array;
		delete [] distances;
	}


	void printCity()
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				cout << array[i][j] << " ";
			}

			cout << "\n";
		}
	}

	void printDistances()
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				cout << distances[i][j] << " ";
			}
			cout << "\n";
		}
	}
};

int main()
{

	ios_base::sync_with_stdio(false);

	int sets;
	cin >> sets;

	while(sets--)
	{
		City city;
		city.printDistances();
	}

	return 0;
}