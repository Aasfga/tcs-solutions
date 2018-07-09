#include <iostream>
#include <limits.h>
#include <vector>

int next_power(int n, int pow = 2)
{
	int x = 1;

	while(x < n)
		x *= pow;
	return x;
}

struct Node
{
	bool correct[4] = {true, true, true, true};
	int left[2] = {INT32_MAX, INT32_MAX};
	int right[2] = {INT32_MAX, INT32_MAX};
};

class CardsSimulator
{

	std::vector<Node> nodes;
	int first_leaf;


	void prepare_node(int x)
	{

		for(bool &i : nodes[x].correct)
			i = false;
		nodes[x].left[0] = nodes[2 * x].left[0];
		nodes[x].left[1] = nodes[2 * x].left[1];
		nodes[x].right[0] = nodes[2 * x + 1].right[0];
		nodes[x].right[1] = nodes[2 * x + 1].right[1];
	}


	void correct_double(int x)
	{
		if(x == 0)
			return;
		prepare_node(x);
		Node &crr = nodes[x];
		Node &left = nodes[2 * x];
		Node &right = nodes[2 * x + 1];


		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 2; j++)
			{
				crr.correct[2 * i + j] = left.right[i] <= right.left[j];
			}
		}

		correct(x / 2);
	}

	void correct(int x)
	{
		if(x == 0)
			return;
		prepare_node(x);
		Node &crr = nodes[x];
		Node &left = nodes[2 * x];
		Node &right = nodes[2 * x + 1];


		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 2; j++)
			{
				for(int k = 0; k < 2; k++)
				{

					crr.correct[i] = crr.correct[i] || (left.correct[2 * (i / 2) + j] &&
					                                    right.correct[2 * k + i % 2] &&
					                                    left.right[j] <= right.left[k]);

				}
			}
		}


		correct(x / 2);
	}

public:

	explicit CardsSimulator(int n)
	{
		n = next_power(n);
		first_leaf = n;
		nodes = std::vector<Node>(2 * n);
	}

	void change_card(int x, int a, int b)
	{
		x = first_leaf + x;
		Node *crr = &nodes[x];
		crr->left[0] = a;
		crr->left[1] = b;
		crr->right[0] = a;
		crr->right[1] = b;

		correct_double(x / 2);
	}

	void swap_cards(int i, int j)
	{
		int values_i[2] = {nodes[first_leaf + i].left[0], nodes[first_leaf + i].left[1]};
		int values_j[2] = {nodes[first_leaf + j].left[0], nodes[first_leaf + j].left[1]};
		change_card(i, values_j[0], values_j[1]);
		change_card(j, values_i[0], values_i[1]);
	}

	bool is_increasing()
	{
		bool res = false;
		for(bool i : nodes[1].correct)
		{
			res = res || i;
		}

		return res;
	}
};

int main()
{
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int cards;
		int swaps;
		int a, b;
		std::cin>>cards;
		CardsSimulator simulator(cards);


		for(int i = 0; i < cards; i++)
		{
			std::cin>>a>>b;
			simulator.change_card(i, a, b);
		}

		std::cin>>swaps;

		while(swaps--)
		{
			std::cin>>a>>b;
			simulator.swap_cards(a - 1, b - 1);
			if(simulator.is_increasing())
				std::cout<<"TAK\n";
			else
				std::cout<<"NIE\n";
		}
	}
	return 0;
}