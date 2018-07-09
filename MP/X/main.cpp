#include <iostream>
#include <stack>


struct Node
{
	int left = 0;
	int right = 0;
	int sum = 0;


	Node()
	{}

	Node(int l, int r)
	{
		left = l;
		right = r;
	}

	bool isFilled()
	{
		return right - left + 1 == sum;
	}
};

class IntervalTree
{
	int size;
	Node *tree;


	int getSize(int size)
	{
		int i = 1;

		while(i < size)
			i *= 2;

		return 2 * i;
	}

	void constructTree(int i)
	{
		if(i >= size)
			return;

		int p = i / 2;

		if(i % 2 == 0)
		{
			tree[i].left = tree[p].left;
			tree[i].right = (tree[p].left + tree[p].right) / 2;
		}
		else
		{
			tree[i].right = tree[p].right;
			tree[i].left = (tree[p].left + tree[p].right) / 2 + 1;
		}

		constructTree(2 * i);
		constructTree(2 * i + 1);
	}

	void constructTree()
	{
		tree[1].left = 0;
		tree[1].right = size / 2 - 1;

		constructTree(2);
		constructTree(3);
	}

	void add(int node, int a)
	{
		int i = node;
		while(i >= 1)
		{
			tree[i].sum += a;

			i /= 2;
		}
	}


	int leftLine(int a)
	{
		int sum = 0;
		int i = 1;

		while(tree[i].left != a || tree[i].right != a)
		{
			if(tree[2 * i].right < a)
			{
				sum -= tree[2 * i].sum;
				i = 2 * i + 1;
			}
			else
				i = 2 * i;
		}
		return sum;
	}

	int rightLine(int b)
	{
		int sum = 0;
		int i = 1;

		while(tree[i].left != b || tree[i].right != b)
		{
			if(tree[2 * i + 1].left > b)
			{
				sum -= tree[2 * i + 1].sum;
				i = 2 * i;
			}
			else
				i = 2 * i + 1;
		}
		return sum;
	}


public:

	IntervalTree(int s)
	{
		size = getSize(s);
		tree = new Node[size];
		constructTree();
	}

	bool reserve(int seat)
	{
		int node = size / 2 + seat;

		if(tree[node].sum != 0)
			return false;
		else
		{
			add(node, 1);
			return true;
		}
	}

	bool cancel(int seat)
	{
		int node = size / 2 + seat;

		if(tree[node].sum == 0)
			return false;
		else
		{
			add(node, -1);
			return true;
		}
	}

	int sum(int a, int b)
	{

		int sum = tree[1].sum;

		sum += leftLine(a);
		sum += rightLine(b);

		return sum;
	}

	int next(int a)
	{
		int i = size / 2 + a;

		while(i >= 1)
		{
			if(i % 2 == 0 && !tree[i + 1].isFilled())
			{
				i = i + 1;
				break;
			}
			else
				i /= 2;
		}

		if(i == 0)
			return size;

		while(tree[i].left != tree[i].right)
		{
			if(!tree[2 * i].isFilled())
				i *= 2;
			else
				i = 2 * i + 1;
		}

		return i - size/2;
	}

	~IntervalTree()
	{
		delete[] tree;
	}
};

int main()
{
	int sets;
	std::cin >> sets;

	while(sets--)
	{
		int n, k;
		std::cin >> n >> k;
		IntervalTree tree(n);

		while(k--)
		{
			std::string task;
			int a, b;

			std::cin >> task >> a;

			if(task == "RESERVE")
			{
				if(!tree.reserve(a - 1))
					std::cout << "ERROR\n";
			}
			else if(task == "CANCEL")
			{
				if(!tree.cancel(a - 1))
					std::cout << "ERROR\n";

			}
			else if(task == "SUM")
			{
				std::cin >> b;

				std::cout << tree.sum(a - 1, b - 1) << "\n";
			}
			else if(task == "NXT")
			{
				b = tree.next(a - 1);
				if(b >= n)
					std::cout << "ERROR\n";
				else
					std::cout << b + 1 << "\n";
			}
			else if(task == "MAX")
			{
				std::cin >> b;
				std::cout << "EXCEPTION\n";
			}

		}
	}

	return 0;
}