#include <iostream>

using namespace std;


int find(int *array, int right, int x)
{
	int i = 0;
	int j = right;

	while(i <= j)
	{

		if(array[i] == x)
		{
			return i;
		}
		else if(array[j] == x)
		{
			return j;
		}

		i++;
		j--;
	}

	return -1;
}

void readArray(int *array, int size)
{
	for(int i = 0; i < size; i++)
	{
		cin >> array[i];
	}
}

template<class C>
void writeArray(C *array, int size)
{
	for(int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	cout << "\n";
}

void preorderInorder(int *preorder, int *inorder, int *postorder, char *whichSide, int right, char side, int depth)
{
	if(right < 0)
	{
		return;
	}

	int pos = find(inorder, right, *preorder);
	postorder[right] = depth;
	whichSide[right] = side;

	preorderInorder(preorder + 1, inorder, postorder, whichSide, pos - 1, 'L', depth + 1);
	preorderInorder(preorder + pos + 1, inorder + pos + 1, postorder + pos, whichSide + pos, right - pos - 1, 'R',
	                depth + 1);
}

void postorderInorder(int *postorder, int *inorder, int *preorder, char *whichSide, int right, char side, int depth)
{
	if(right < 0)
	{
		return;
	}

	int pos = find(inorder, right, *(postorder + right));
	preorder[0] = depth;
	whichSide[0] = side;

	postorderInorder(postorder, inorder, preorder + 1, whichSide + 1, pos - 1, 'L', depth + 1);
	postorderInorder(postorder + pos, inorder + pos + 1, preorder + pos + 1, whichSide + pos + 1, right - pos - 1, 'R',
	                 depth + 1);
}

bool postorderPreorder(int *postorder, int *preorder, int *inorder, char *whichSide, int right, char side, int depth)
{
	if(right < 0)
	{
		return true;
	}
	if(right == 0)
	{
		inorder[0] = depth;
		whichSide[0] = side;
		return true;
	}


	int pos = find(postorder, right, preorder[1]);
	if(pos == right - 1)
	{
		return false;
	}

	inorder[pos + 1] = depth;
	whichSide[pos + 1] = side;


	bool b = postorderPreorder(postorder, preorder + 1, inorder, whichSide, pos, 'L', depth + 1);

	if(!b)
	{
		return false;
	}

	b = postorderPreorder(postorder + pos + 1,
	                      preorder + pos + 2,
	                      inorder + pos + 2,
	                      whichSide + pos + 2,
	                      right - pos - 2,
	                      'R',
	                      depth + 1);

	return b;
}

int main()
{

	int sets;
	cin >> sets;

	while(sets--)
	{
		int size;
		int *firstOrder;
		int *secondOrder;
		string firstName;
		string secondName;

		cin >> size;
		firstOrder = new int[size];
		secondOrder = new int[size];

		int *ansOrder = new int[size];
		char *whichSide = new char[size];

		cin>> firstName;
		readArray(firstOrder, size);
		cin>> secondName;
		readArray(secondOrder, size);


		if(firstName == "PREORDER")
		{
			if(secondName == "POSTORDER")
			{
				swap(firstOrder, secondOrder);
				bool b = postorderPreorder(firstOrder, secondOrder, ansOrder, whichSide, size - 1, 'K', 0);

				if(b)
				{
					writeArray(ansOrder, size);
					writeArray(whichSide, size);
				}
				else
				{
					cout << "ERROR" << endl;
				}
			}
			else
			{
				preorderInorder(firstOrder, secondOrder, ansOrder, whichSide, size - 1, 'K', 0);
				writeArray(ansOrder, size);
				writeArray(whichSide, size);
			}
		}
		else if(firstName == "POSTORDER")
		{
			if(secondName == "PREORDER")
			{
				bool b = postorderPreorder(firstOrder, secondOrder, ansOrder, whichSide, size - 1, 'K', 0);

				if(b)
				{
					writeArray(ansOrder, size);
					writeArray(whichSide, size);
				}
				else
				{
					cout << "ERROR" << endl;
				}
			}
			else
			{
				postorderInorder(firstOrder, secondOrder, ansOrder, whichSide, size - 1, 'K', 0);
				writeArray(ansOrder, size);
				writeArray(whichSide, size);
			}
		}
		else if(firstName == "INORDER")
		{
			swap(firstOrder, secondOrder);

			if(secondName == "PREORDER")
			{
				preorderInorder(firstOrder, secondOrder, ansOrder, whichSide, size - 1, 'K', 0);
				writeArray(ansOrder, size);
				writeArray(whichSide, size);
			}
			else
			{
				postorderInorder(firstOrder, secondOrder, ansOrder, whichSide, size - 1, 'K', 0);
				writeArray(ansOrder, size);
				writeArray(whichSide, size);
			}
		}


		delete [] firstOrder;
		delete [] secondOrder;
		delete [] ansOrder;
		delete [] whichSide;
	}


	return 0;
}