#include <iostream>
#include <vector>

using namespace std;

class MinMaxHeap
{
	vector<int> array;


	int mother(int i)
	{
		i = i / 2;
		if(i % 2 == 1)
			i--;


		return i;
	}

	int father(int i)
	{
		i = i / 2;

		if(i % 2 == 0)
			i++;


		return i;
	}

	int leftMax(int i)
	{
		if(i % 2 == 0)
			return 2 * i + 1;
		else
			return 2 * i - 1;
	}

	int rightMax(int i)
	{
		if(i % 2 == 0)
			return 2 * i + 3;
		else
			return 2 * i + 1;
	}

	int leftMin(int i)
	{
		if(i % 2 == 0)
			return 2 * i;
		else
			return 2 * i - 2;
	}

	int rightMin(int i)
	{
		if(i % 2 == 0)
			return 2 * i + 2;
		else
			return 2 * i;
	}

	int getMin(int i)
	{
		if(leftMin(i) >= array.size())
			return -1;
		else if(rightMin(i) >= array.size())
			return leftMin(i);
		else
			return array[leftMin(i)] < array[rightMin(i)] ? leftMin(i) : rightMin(i);
	}

	int brother(int i)
	{
		if(i % 2 == 0)
			return i + 1;
		else
			return i - 1;
	}

	bool hasBrother(int i)
	{
		 if(i % 2 == 0)
			 return i + 1 < array.size();
		 else
			 return true;
	}

	bool wrongOrderOfBrothers(int i)
	{
		if(i % 2 == 0)
			return array[i] > array[i + 1];
		else
			return array[i] < array[i - 1];
	}

	int getMax(int i)
	{
		if(leftMax(i) >= array.size())
			return -1;
		else if(rightMax(i) >= array.size())
			return leftMax(i);
		else
			return array[leftMax(i)] > array[rightMax(i)] ? leftMax(i) : rightMax(i);
	}

public:

	MinMaxHeap()
	{
		array.push_back(-1);
		array.push_back(-1);
	};

	void insert(int x)
	{
		int i = (int) array.size();
		array.push_back(x);

		if(i % 2 == 1 && array[i - 1] > array[i])
		{
			swap(array[i], array[i - 1]);
			i--;
		}

		while(i > 3)
		{
			if(array[i] < array[mother(i)])
			{
				swap(array[i], array[mother(i)]);
				i = mother(i);
			}
			else if(array[i] > array[father(i)])
			{
				swap(array[i], array[father(i)]);
				i = father(i);
			}
			else
				break;
		}
	}

	int max()
	{
		if(size() == 1)
			return array[2];
		else
			return array[3];
	}

	int min()
	{
		return array[2];
	}

	void popMin()
	{
		if(size() == 0)
			return;

		int i = 2;
		swap(array[2], array[array.size() - 1]);
		array.pop_back();

		while(true)
		{
			if(hasBrother(i) && wrongOrderOfBrothers(i))
			{
				swap(array[i], array[brother(i)]);
			}

			int min = getMin(i);

			if(min != -1 && array[i] > array[min])
			{
				swap(array[i], array[min]);
				i = min;
			}
			else
				break;
		}

		if(hasBrother(i) && wrongOrderOfBrothers(i))
		{
			swap(array[i], array[brother(i)]);
			i = brother(i);
		}

	}

	void popMax()
	{
		if(size() == 0)
			return;

		int i = 3;
		swap(array[3], array[array.size() - 1]);
		array.pop_back();

		while(true)
		{
			if(hasBrother(i) && wrongOrderOfBrothers(i))
			{
				swap(array[i], array[brother(i)]);
			}

			int max = getMax(i);

			if(max != -1 && array[i] < array[max])
			{
				swap(array[i], array[max]);
				i = max;
			}
			else
				break;
		}

		if(hasBrother(i) && wrongOrderOfBrothers(i))
		{
			swap(array[i], array[brother(i)]);
			i = brother(i);
		}
	}

	int size()
	{
		return (int) (array.size() - 2);
	}
};

class RichCreator
{
	MinMaxHeap min;
	MinMaxHeap max;
	int maxSize;
	bool dualHeaps = false;


public:

	RichCreator(int n)
	{
		maxSize = n;
	}

	void put(int x)
	{
		if(!dualHeaps && min.size() < 2 * maxSize)
		{
			min.insert(x);
		}
		else
		{
			if(x < min.max())
			{
				min.popMax();
				min.insert(x);
			}
			else if(x > max.min())
			{
				max.popMin();
				max.insert(x);
			}
		}

		if(min.size() == 2*maxSize)
		{
			dualHeaps = true;
			for(int i = 0; i < maxSize; i++)
			{
				max.insert(min.max());
				min.popMax();
			}
		}
	}

	long get()
	{
		long sum = 0;

		if(dualHeaps)
		{
			sum = max.max() - min.min();
			min.popMin();
			max.popMax();
		}
		else
		{
			sum = min.max() - min.min();
			min.popMax();
			min.popMin();
		}

		return sum;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);


	int sets;
	cin>> sets;

	while(sets--)
	{
		int n;
		cin>> n;
		RichCreator creator(n);
		long sum = 0;

		while(n--)
		{
			int k;
			cin>> k;
			while(k--)
			{
				int x;
				cin>> x;
				creator.put(x);
			}
			sum += creator.get();
		}

		cout<< sum<< "\n";
	}


	return 0;
}