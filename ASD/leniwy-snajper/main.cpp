#include <iostream>
#include <vector>
#include <algorithm>

#define LEFT 1
#define RIGHT (-1)
#define UPPER RIGHT
#define LOWER LEFT


struct point
{
	long long x;
	long long y;

	point() = default;

	point(long long int x, long long int y) : x(x), y(y)
	{}
};

typedef point vector;

vector create_vector(point a, point b)
{
	return {b.x - a.x, b.y - a.y};
}

int sgn(long x)
{
	if(x > 0)
		return 1;
	else if(x == 0)
		return 0;
	else
		return -1;
}

int get_side(point a, point b, point x)
{
	vector u = create_vector(a, b);
	vector v = create_vector(a, x);

	return sgn(u.x * v.y - u.y * v.x);
}

std::vector<point> convex_hull(std::vector<point> &points, int side)
{
	std::sort(points.begin(), points.end(), [](point a, point b){ return a.x < b.x;});
	std::vector<point> hull;
	if(points.size() < 2)
	{
		hull = points;
		return hull;
	}
	hull.push_back(points[0]);
	hull.push_back(points[1]);
	for(int i = 2; i < points.size(); i++)
	{
		point x = points[i];
		while(hull.size() > 1)
		{
			u_long s = hull.size() - 1;
			point a = hull[s - 1];
			point b = hull[s];
			if(get_side(a, b, x) != side)
				hull.pop_back();
			else
				break;
		}
		hull.push_back(x);
	}

	return hull;
}



bool _are_crossing(std::vector<point> &first, std::vector<point> &second, int side)
{
	if(first.size() == 1)
		return true;
	point a = second[0];
	point b = second[1];
	int iter = 2;

	for(auto p : first)
	{
		while(p.x > b.x)
		{
			a = b;
			b = second[iter++];
		}

		if(get_side(a, b, p) != side)
			return true;
	}
	return false;
}


bool are_crossing(std::vector<point> &upper, std::vector<point> &lower)
{
	return _are_crossing(upper, lower, LEFT) || _are_crossing(lower, upper, RIGHT);
}

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int n;
		std::cin>> n;
		std::vector<point> upper;
		std::vector<point> lower;
		while(n--)
		{
			int x, a, b;
			std::cin>> x>> a>> b;
			lower.emplace_back(x, a);
			upper.emplace_back(x, b);
		}
		upper = convex_hull(upper, LOWER);
		lower = convex_hull(lower, UPPER);
		if(are_crossing(upper, lower))
			std::cout<< "NIE\n";
		else
			std::cout<< "TAK\n";
	}
	return 0;
}