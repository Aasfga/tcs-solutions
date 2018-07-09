#include <iostream>


class CrazyOperator
{
	struct Node
	{
		Node *left = nullptr;
		Node *right = nullptr;
		Node *parent;
		int value;
		int size;
		short rev = 1;

		Node(int value, int size, Node *parent) : value(value), size(size), parent(parent)
		{}

		int l_size()
		{
			if(left == nullptr)
				return 0;
			return left->size;
		}

		int r_size()
		{
			if(right == nullptr)
				return 0;
			return right->size;
		}

		void calc_size()
		{
			size = l_size() + r_size() + 1;
		}

		bool is_left(Node *node)
		{
			return left == node;
		}

		bool is_right(Node *node)
		{
			return right == node;
		}

		void swap(Node *old, Node *new_)
		{
			new_->parent = this;
			if(left == old)
				left = new_;
			else if(right == old)
				right = new_;
			else
				int i = 1/0;
		}

		void reverse()
		{
			if(left != nullptr)
				left->rev *= -1;
			if(right != nullptr)
				right->rev *= -1;
			std::swap(left, right);
			rev = 1;
		}
	};


	Node *root;
	int size;

	bool is_on_top(Node *node)
	{
		return root->right == node;
	}

	bool is_zig_zig(Node *x)
	{
		Node *p = x->parent;
		Node *g = p->parent;


		if(is_on_top(p))
			return false;

		if(p->is_left(x) && g->is_left(p))
			return true;
		return p->is_right(x) && g->is_right(p);
	}

	bool is_zig_zag(Node *x)
	{
		Node *p = x->parent;
		Node *g = p->parent;

		if(is_on_top(p))
			return false;

		if(p->is_left(x) && g->is_right(p))
			return true;
		return p->is_right(x) && g->is_left(p);
	}


//	z lewa na prawo
	void left_zig(Node *crr)
	{
		Node *crr_right = crr -> right;
		Node *p = crr -> parent;
		p->parent->swap(p, crr);
		p->parent = crr;
		crr -> right = p;
		p -> left = crr_right;
		if(crr_right != nullptr)
			crr_right -> parent = p;
		p->calc_size();
		crr->calc_size();
	}

	void right_zig(Node *crr)
	{
		Node *crr_left = crr->left;
		Node *p = crr->parent;
		p->parent->swap(p, crr);
		p->parent = crr;
		crr->left = p;
		p->right = crr_left;
		if(crr_left != nullptr)
			crr_left->parent = p;
		p->calc_size();
		crr->calc_size();
	}

	void zig_zig(Node *x)
	{
		Node *p = x->parent;
		Node *g = p->parent;

		if(p->is_left(x))
		{
			left_zig(p);
			left_zig(x);
		}
		else
		{
			right_zig(p);
			right_zig(x);
		}
	}

	void zig_zag(Node *x)
	{
		Node *p = x->parent;
		Node *g = p->parent;

		if(p->is_left(x))
		{
			left_zig(x);
			right_zig(x);
		}
		else
		{
			right_zig(x);
			left_zig(x);
		}
	}

	Node *find(int x)
	{
		Node *crr = root->right;

		while(crr->l_size() + 1 != x)
		{
			if(x <= crr->l_size())
				crr = crr->left;
			else
			{
				x -= crr->l_size() + 1;
				crr = crr->right;
			}
			if(crr->rev == -1)
				crr -> reverse();
		}

		return crr;
	}

	void splay(int x)
	{
		Node *node = find(x);

		while(!is_on_top(node))
		{
			if(is_zig_zig(node))
				zig_zig(node);
			else if(is_zig_zag(node))
				zig_zag(node);
			else if(node->parent->is_left(node))
				left_zig(node);
			else
				right_zig(node);
		}
	}

	void reverse_prefix(int j)
	{
		splay(j+1);
		root->right->left->rev *= -1;
	}

public:

	void reverse(int i, int j)
	{
		reverse_prefix(j);
		int end = get(1);
		reverse_prefix(j - i + 1);
		int begin = get(1);
		reverse_prefix(j);
		std::cout<< begin<< " "<< end<< "\n";
	}

	int get(int i)
	{
		splay(i);
		return root->right->value;
	}

	void print()
	{
		for(int i = 1; i <= size; i++)
			std::cout<< get(i)<< " ";
		std:: cout<< "\n";
	}


	explicit CrazyOperator(int n) : root(new Node(0, n + 1, nullptr)), size(n)
	{
		Node *crr = root;
		for(int i = 1; i <= n + 1; i++)
		{
			crr->right = new Node(i, n + 2 - i, crr);
			crr = crr->right;
		}
	}

	void delete_tree(Node *node)
	{
		if(node == NULL)
			return;
		delete_tree(node ->left);
		delete_tree(node->right);
		delete node;
	}

	~CrazyOperator()
	{
		delete_tree(root);
	}
};

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int n;
		std::cin>> n;
		int revs;
		std::cin>> revs;

		CrazyOperator co(n);
		while(revs--)
		{
			int a, b;
			std::cin>> a>> b;
			co.reverse(a, b);
		}
	}
}

