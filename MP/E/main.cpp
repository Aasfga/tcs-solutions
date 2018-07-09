#include <iostream>
#include <stack>
#include <queue>

using namespace std;


class Tree
{
	int *leftChildren;
	int *rightChildren;
	int *depths;
	int size;


	friend istream &operator>>(istream &in, Tree &tree);

	friend ostream &operator<<(ostream &out, Tree &tree);


	bool isLeaf(int i)
	{
		return leftChildren[i] == rightChildren[i];
	}

	void setDepth()
	{
		stack<int> nodes;
		stack<int> depths;
		int i = 0;
		int depth = 0;

		while(i != -1 || !nodes.empty())
		{
			if(i == -1)
			{
				i = nodes.top();
				nodes.pop();
				depth = depths.top();
				depths.pop();
			}

			this->depths[i] = depth;

			if(rightChildren[i] != -1)
			{
				nodes.push(rightChildren[i]);
				depths.push(depth + 1);
			}
			i = leftChildren[i];
			depth++;
		}
	}

public:

	Tree(int size) : size(size)
	{
		leftChildren = new int[size];
		rightChildren = new int[size];
		depths = new int[size];

		cin >> *this;
		setDepth();
	}

	void inOrder()
	{
		stack<int> nodes;
		int i = 0;

		while(i != -1 || !nodes.empty())
		{
			if(i == -1)
			{
				i = nodes.top();
				nodes.pop();
				cout << i << " ";
				i = rightChildren[i];
			}

			if(i != -1)
			{
				nodes.push(i);
				i = leftChildren[i];
			}
		}
		cout << "\n";
	}

	void depthInOrder()
	{
		{
			stack<int> nodes;
			int i = 0;

			while(i != -1 || !nodes.empty())
			{
				if(i == -1)
				{
					i = nodes.top();
					nodes.pop();
					if(isLeaf(i))
					{
						cout << i << ":" << depths[i] << " ";
					}
					i = rightChildren[i];
				}

				if(i != -1)
				{
					nodes.push(i);
					i = leftChildren[i];
				}
			}
			cout << "\n";
		}
	}

	void preOrder()
	{
		stack<int> nodes;
		int i = 0;

		while(i != -1 || !nodes.empty())
		{
			if(i != -1)
			{
				cout << i << " ";
				nodes.push(i);
				i = leftChildren[i];
			}

			if(i == -1)
			{
				i = nodes.top();
				nodes.pop();
				i = rightChildren[i];
			}
		}

		cout << "\n";
	}

	void depthPreOrder()
	{
		stack<int> nodes;
		int i = 0;

		while(i != -1 || !nodes.empty())
		{
			if(i != -1)
			{
				if(isLeaf(i))
				{
					cout << i << ":" << depths[i] << " ";
				}
				nodes.push(i);
				i = leftChildren[i];
			}

			if(i == -1)
			{
				i = nodes.top();
				nodes.pop();
				i = rightChildren[i];
			}
		}

		cout << "\n";
	}

	void postOrder()
	{
		stack<int> post;
		stack<int> nodes;
		nodes.push(0);
		int i;

		while(!nodes.empty())
		{
			int node = nodes.top();
			post.push(node);
			nodes.pop();

			if(leftChildren[node] != -1)
			{
				nodes.push(leftChildren[node]);
			}
			if(rightChildren[node] != -1)
			{
				nodes.push(rightChildren[node]);
			}
		}

		while(!post.empty())
		{
			cout << post.top() << " ";
			post.pop();
		}

		cout << "\n";
	}

	void depthPostOrder()
	{
		stack<int> post;
		stack<int> nodes;
		nodes.push(0);
		int i;

		while(!nodes.empty())
		{
			int node = nodes.top();
			post.push(node);
			nodes.pop();

			if(leftChildren[node] != -1)
				nodes.push(leftChildren[node]);
			if(rightChildren[node] != -1)
				nodes.push(rightChildren[node]);
		}

		while(!post.empty())
		{
			i = post.top();
			if(isLeaf(i))
			{
				cout << i << ":" << depths[i] << " ";
			}
			post.pop();
		}

		cout<< "\n";

	};

	~Tree()
	{
		delete[] leftChildren;
		delete[] rightChildren;
		delete[] depths;
	}
};

ostream &operator<<(ostream &out, Tree &tree)
{
	stack<int> nodes;

	for(int i = 0; i != -1 || !nodes.empty();)
	{
		if(i == -1)
		{
			i = nodes.top();
			nodes.pop();
		}
		cout << string(tree.depths[i], '\t') + to_string(i) << endl;
		if(tree.rightChildren[i] != -1)
		{
			nodes.push(tree.rightChildren[i]);
		}
		i = tree.leftChildren[i];
	}


	return out;
}


istream &operator>>(istream &in, Tree &tree)
{
	for(int i = 0; i < tree.size; i++)
	{
		cin >> tree.leftChildren[i] >> tree.rightChildren[i];
	}

	return in;
}



int main()
{
	int sets;
	cin>> sets;

	while(sets--)
	{
		int n;
		cin>> n;
		Tree tree(n);

		string order;
		cin>> order;

		if(order == "INORDER")
		{
			tree.inOrder();
			tree.depthInOrder();
		}
		else if(order == "POSTORDER")
		{
			tree.postOrder();
			tree.depthPostOrder();
		}
		else
		{
			tree.preOrder();
			tree.depthPreOrder();
		}

	}
	return 0;
}