#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>
#include <set>

#define LOWER 0
#define UPPER 1

long square(long a)
{
	return a*a;
}

struct point
{
	long x;
	long y;

	bool operator==(const point &rhs) const
	{
		return x == rhs.x &&
		       y == rhs.y;
	}

	long dist(const point &other) const
	{
		long _x = x - other.x;
		long _y = y - other.y;
		return square(_x) + square(_y);
	}
};

struct half_circle;

struct circle
{
	point center;
	long radius;
	int id;

	bool operator==(const circle &rhs) const
	{
		return center == rhs.center &&
		       radius == rhs.radius &&
		       id == rhs.id;
	}

	bool operator<(const circle &rhs) const
	{
		return id < rhs.id;
	}

	circle() = default;

	circle(const circle &other) = default;

	circle(point center, long radius, int id) : center(center), radius(radius), id(id)
	{}

	half_circle lower();

	half_circle upper();
};


struct half_circle
{
	circle parent;
	int type;

	half_circle() = default;

	half_circle(circle parent, int type) : parent(parent), type(type)
	{}

	bool operator==(const half_circle &rhs) const
	{
		return parent == rhs.parent &&
		       type == rhs.type;
	}

	bool operator!=(const half_circle &rhs) const
	{
		return !(rhs == *this);
	}

	long get_x() const
	{
		return parent.center.x;
	}

	long get_y() const
	{
		return parent.center.y;
	}

	long get_radius() const
	{
		return parent.radius;
	}

	long double calc_y(long double x) const
	{
		x -= get_x();
		long double b = get_y();
		long double r = get_radius();

		long double delta = sqrt(r*r - x*x);

		if(type == UPPER)
			return b + delta;
		else
			return b - delta;
	}

	bool operator<(const half_circle &other) const
	{
		if(parent == other.parent)
			return type < other.type;

		if(get_x() - get_radius() < other.get_x() - other.get_radius())
			return calc_y(other.get_x() - other.get_radius()) < other.get_y();
		else
			return get_y() < other.calc_y(get_x() - get_radius());
	}
};

half_circle circle::lower()
{
	return {*this, LOWER};
}

half_circle circle::upper()
{
	return {*this, UPPER};
}

typedef std::pair<long, circle> event;


bool is_erase(long x, circle &c)
{
	return x == c.center.x + c.radius;
}

void set_parents(std::vector<event> &events, int *parents)
{
	std::set<half_circle> broom;
	circle guard({0, 0}, 10000000, 0);
	broom.insert(guard.lower());
	broom.insert(guard.upper());


	for(auto e : events)
	{
		long x = e.first;
		circle c = e.second;
		if(is_erase(x, c))
		{
			broom.erase(c.lower());
			broom.erase(c.upper());
		}
		else
		{
			half_circle upper = c.upper();
			half_circle lower = c.lower();
			broom.insert(upper);
			broom.insert(lower);
			auto iter = broom.find(upper);
			iter++;
			int id = iter->parent.id;
			if(iter->type == UPPER)
				parents[c.id] = id;
			else
				parents[c.id] = parents[id];
		}

	}
}

int main()
{
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int n;
		std::cin>>n;
		auto *parents = new int[n + 1];
		std::vector<event> events;

		for(int i = 1; i <= n; i++)
		{
			long x, y, r;
			std::cin>>x>>y>>r;
			events.emplace_back(x - r, circle({x, y}, r, i));
			events.emplace_back(x + r, circle({x, y}, r, i));
		}


		std::sort(events.begin(), events.end(), [](event a, event b)
		{ return a.first < b.first; });
		set_parents(events, parents);

		for(int i = 1; i <= n; ++i)
			std::cout<< parents[i]<< "\n";

		delete[] parents;
	}

}
