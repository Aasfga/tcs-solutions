#include <iostream>
#include <stack>

using namespace std;


template<class Key, class Value>
class node
{

	template<class A, class B> friend
	class map;

	template<class A, class B> friend
	class iteratorM;


	Key key;
	Value value;
	node<Key, Value> *left;
	node<Key, Value> *right;
	node<Key, Value> *parent;

public:

	node(Key key, Value value, node<Key, Value> *parent = NULL, node<Key, Value> *left = NULL, node<Key, Value> *right = NULL)
	{
		this->key = key;
		this->value = value;
		this->parent = parent;
		this->left = left;
		this->right = right;
	}

	Key GetKey()
	{
		return key;
	}

	Value GetName()
	{
		return value;
	}

	bool hasTwoChildren()
	{
		return left != nullptr && right != nullptr;
	}


	bool noChildren()
	{
		return right == nullptr && left == nullptr;
	}

	bool hasOneChild()
	{
		return !noChildren() && (left == nullptr || right == nullptr);
	}

	template<class A, class B>
	friend ostream &operator<<(ostream &, node<A, B> &);
};

template<class Key, class Value>
ostream &operator<<(ostream &out, node<Key, Value> &node)
{
	out << node.key << " " << node.value << "\n";
	return out;
}

template<class Key, class Value>
class iteratorM
{
	template<class A, class B> friend
	class map;

	node<Key, Value> *crrNode;

public:

	iteratorM(node<Key, Value> *a)
	{
		crrNode = a;
	}

	node<Key, Value> &operator*()
	{
		return *crrNode;
	};

	node<Key, Value> *operator->()
	{
		return crrNode;
	};

	operator bool()
	{
		return crrNode != nullptr;
	}

	iteratorM<Key, Value> operator++(int)
	{
		if(crrNode == nullptr)
			return *this;

		if(crrNode->right != nullptr)
		{
			crrNode = crrNode->right;

			while(crrNode->left != nullptr)
			{
				crrNode = crrNode->left;
			}
		}
		else
		{
			while(crrNode != nullptr && crrNode->parent != nullptr && crrNode == crrNode->parent->right)
			{
				crrNode = crrNode->parent;
			}
			crrNode = crrNode->parent;
		}

		return *this;
	};

	iteratorM<Key, Value> operator--(int)
	{

		if(crrNode == nullptr)
			return *this;

		if(crrNode->left != nullptr)
		{
			crrNode = crrNode->left;

			while(crrNode->right != nullptr)
			{
				crrNode = crrNode->right;
			}
		}
		else
		{
			while(crrNode != nullptr && crrNode -> parent != nullptr && crrNode == crrNode->parent->left)
			{
				crrNode = crrNode->parent;
			}
			crrNode = crrNode->parent;
		}

		return *this;
	};
};

template<class Key, class Value>
class map
{

	node<Key, Value> *root;

public:

	map()
	{
		root = nullptr;
	}

	void insert(Key key, Value value)
	{
		if(root == nullptr)
		{
			root = new node<Key, Value>(key, value, NULL, NULL);
			return;
		}

		node<Key, Value> *treeNode = root;

		while(true)
		{
			if(treeNode->key > key)
			{
				if(treeNode->left == nullptr)
				{
					treeNode->left = new node<Key, Value>(key, value, treeNode);
					break;
				}
				else
				{
					treeNode = treeNode->left;
				}
			}
			else if(treeNode->key < key)
			{
				if(treeNode->right == nullptr)
				{
					treeNode->right = new node<Key, Value>(key, value, treeNode);
					break;
				}
				else
				{
					treeNode = treeNode->right;
				}
			}
			else
			{
				treeNode->value = value;
				break;
			}

		}

	}


	iteratorM<Key, Value> begin()
	{
		iteratorM<Key, Value> iter(root);

		if(root == nullptr)
		{
			return iter;
		}

		while(iter.crrNode->left != nullptr)
		{
			iter.crrNode = iter.crrNode->left;
		}

		return iter;
	};

	iteratorM<Key, Value> end()
	{
		iteratorM<Key, Value> iter(root);

		if(root == nullptr)
		{
			return iter;
		}

		while(iter.crrNode->right != nullptr)
		{
			iter.crrNode = iter.crrNode->right;
		}

		return iter;
	};

	//TODO możliwa szybsza wersja
	iteratorM<Key, Value> find(Key key)
	{
		node<Key, Value> *treeNode = root;

		while(treeNode != nullptr && treeNode->key != key)
		{
			if(treeNode->key > key)
			{
				treeNode = treeNode->left;
			}
			else
			{
				treeNode = treeNode->right;
			}
		}

		iteratorM<Key, Value> iter(treeNode);
		return iter;
	};

	iteratorM<Key, Value> findOrNext(Key key)
	{
		node<Key, Value> *treeNode = root;
		node<Key, Value> *min = nullptr;

		while(treeNode != nullptr)
		{
			if(treeNode->key >= key)
			{
				if(min == nullptr || min->key > treeNode->key)
				{
					min = treeNode;
				}

				treeNode = treeNode->left;
			}
			else
			{
				treeNode = treeNode->right;
			}
		}


		iteratorM<Key, Value> iter(min);

		return iter;
	};

	void pClean(node<Key, Value> *treeNode)
	{
		if(treeNode->left != nullptr)
		{
			pClean(treeNode->left);
		}
		if(treeNode->right != nullptr)
		{
			pClean(treeNode->right);
		}
		delete treeNode;
	}

	void clean()
	{
		if(root != nullptr)
			pClean(root);
		root = nullptr;
	}

	int size()
	{
		iteratorM<Key, Value> iter = begin();
		int size = 0;

		while(iter)
		{
			size++;
			iter++;
		}

		return size;
	}

	Value &operator[](Key key)
	{
		iteratorM<Key, Value> iter = find(key);

		if(!iter)
		{
			Value value;
			insert(key, value);
			iter = find(key);
		}

		return iter -> value;
	}

	bool erase(iteratorM<Key, Value> iter)
	{
		if(!iter)
		{
			return false;
		}

		if(iter.crrNode == root && !root -> hasTwoChildren())
		{
			if(root->noChildren())
			{
				delete root;
				root = nullptr;
			}
			else if(root->hasOneChild())
			{
				if(root->left != nullptr)
				{
					root = root->left;
				}
				else
				{
					root = root->right;
				}

				delete root->parent;
				root->parent = nullptr;
			}
		}
		else
		{
			if(iter.crrNode -> noChildren())
			{
				if(iter.crrNode -> parent -> left == iter.crrNode)
					iter.crrNode -> parent -> left = nullptr;
				else
					iter.crrNode -> parent -> right = nullptr;

				delete iter.crrNode;
				iter.crrNode = nullptr;
			}
			else if(iter.crrNode -> hasOneChild())
			{
				node<Key, Value> *child = iter.crrNode->left == nullptr ? iter.crrNode->right : iter.crrNode->left;
				child -> parent = iter.crrNode -> parent;

				if(iter.crrNode->parent->left == iter.crrNode)
					iter.crrNode->parent->left = child;
				else
					iter.crrNode->parent->right = child;

				delete iter.crrNode;
				iter.crrNode = nullptr;
			}
			else
			{
				iteratorM<Key, Value> next(iter.crrNode);
				next++;
				iter.crrNode -> key = next.crrNode -> key;
				iter.crrNode -> value = next.crrNode -> value;

				erase(next);
			}
		}


		return true;
	}

	bool erase(Key key)
	{
		iteratorM<Key, Value> iter = find(key);
		return erase(iter);
	}

	void print()
	{
		if(root == nullptr)
			cout<< "EMPTY\n";


		stack<node<Key, Value> *> nodes;
		node<Key, Value> *i = root;

		while(i != nullptr || !nodes.empty())
		{
			if(i == nullptr)
			{
				i = nodes.top();
				nodes.pop();
				cout << (*i);
				i = i->right;
			}

			if(i != nullptr)
			{
				nodes.push(i);
				i = i->left;
			}
		}
	}

	void print(Key first, Key second)
	{
		if(root == nullptr)
			cout<< "EMPTY\n";
		iteratorM<Key, Value> iter = findOrNext(first);

		while(iter && iter.crrNode -> key <= second)
		{
			cout<< (*iter);
			iter++;
		}
	}

	~map()
	{
		clean();
	}
};


