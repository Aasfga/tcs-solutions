#include <iostream>

using namespace std;

class Tree
{
	int root;
	int size;
	int *right;
	int *left;

public:

	Tree()
	{
		cin>> size>> root;
		left = new int[size + 1];
		right = new int[size + 1];

		right[0] = 0;
		left[0] = 0;

		for(int i = 1; i <= size; i++)
		{
			cin>> left[i]>> right[i];
		}
	}

	Tree(int size, int root, int *left, int *right)
	{
		this-> root = root;
		this-> size = size;
		this-> left = new int[size + 1];
		this-> right = new int[size + 1];

		this-> left[0] = 0;
		this-> right[0] = 0;

		for(int i = 0; i <= size; i++)
		{
			this -> left[i + 1] = left[i];
			this -> right[i + 1] = right[i];
		}

	}

	void treeTraversal()
	{
		int leftTester = left[root];
		int rightTester = right[root];

		int parent = 0;
		int crr = root;

		do
		{
			if(crr != 0 && left[crr] != 0)
				cout<< crr<< " ";


			int tmp = left[crr];
			swap(left[crr], right[crr]);
			right[crr] = parent;
			parent = crr;
			crr = tmp;
		} while(left[root] != leftTester || right[root] != rightTester);

		cout<< root<< "\n";

	}

	~Tree()
	{
		delete [] right;
		delete [] left;
	}
};


int main()
{
	ios_base::sync_with_stdio(false);

	int tasks;
	cin>> tasks;
	while(tasks--)
	{
		Tree tree;
		tree.treeTraversal();
	}

	return 0;
}