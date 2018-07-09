#include <iostream>
#include <vector>

unsigned int next_power(unsigned int n, int pow = 2)
{
	unsigned int x = 1;

	while(x < n)
	{
		x *= pow;
	}

	return x;
}


class SeatSupervisor
{
	struct Section
	{
		int ends[2] = {0, 0};
		int mod = 0;
		int max = 0;
	};

	int seats;
	std::vector<Section> section_tree;

	void set_ends(int x, int s, int e)
	{
		section_tree[x].ends[0] = s;
		section_tree[x].ends[1] = e;
		if(s == e)
			return;
		int mid = (s + e) / 2;
		set_ends(2 * x, s, mid);
		set_ends(2 * x + 1, mid + 1, e);
	}


	void add_passengers(int x, int a, int b, int how_many)
	{
		Section *crr = &section_tree[x];
		if(crr->ends[0] > b || crr->ends[1] < a)
			return;
		if(crr->ends[0] == a && crr->ends[1] == b)
		{
			crr->mod += how_many;
			crr->max = crr->max + how_many;
			return;
		}
		int s = (crr->ends[0] + crr->ends[1]) / 2;
		add_passengers(2 * x, a, std::min(b, s), how_many);
		add_passengers(2 * x + 1, std::max(a, s + 1), b, how_many);

		crr->max = std::max(section_tree[2 * x].max, section_tree[2 * x + 1].max) + crr->mod;
	}

public:

	SeatSupervisor(unsigned int sections, int seats) : seats(seats)
	{
		sections = next_power(sections);
		section_tree = std::vector<Section>(2 * sections);
		set_ends(1, 1, sections);
	}

	int add_passengers(int a, int b, int how_many)
	{
		add_passengers(1, a, b, how_many);
		if(section_tree[1].max > seats)
		{
			add_passengers(1, a, b, -how_many);
			return -1;
		}
		return 0;
	}
};


int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int cities;
		int seats;
		int tickets;
		std::cin>> cities>> seats>> tickets;
		SeatSupervisor supervisor(cities - 1, seats);
		while(tickets--)
		{
			int a, b, how_many;
			std::cin>> a>> b>> how_many;
			if(supervisor.add_passengers(a, b - 1, how_many) < 0)
				std::cout<< "N\n";
			else
				std::cout<< "T\n";
		}
	}

	return 0;
}