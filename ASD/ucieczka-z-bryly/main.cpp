#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <limits>

long double sgn(long double x)
{
	if(x > 0)
		return 1;
	else if(x == 0)
		return 0;
	else
		return -1;
}

struct point
{
	long double x;
	long double y;
	long double z;

	point() = default;

	point(long double x, long double y, long double z) : x(x), y(y), z(z)
	{}
};

typedef point vector;


vector get_vector(point a, point b)
{
	return {b.x - a.x, b.y - a.y, b.z - a.z};
}

vector normal_vector(vector u, vector v)
{
	return {u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x};
}

struct plane
{
	long double a;
	long double b;
	long double c;
	long double d;

	plane(vector n, point p) : a(n.x), b(n.y), c(n.z), d(-n.x*p.x - n.y*p.y - n.z*p.z)
	{}

	long double get_side(point p)
	{
		return sgn(a*p.x + b*p.y + c*p.z + d);
	}

	long double get_distance(point p)
	{
		long double counter = labs(a*p.x + b*p.y + c*p.z + d);
		long double denominator = sqrt(a*a + b*b + c*c);
		return counter/denominator;
	}
};

bool is_plane(point a, point b, point c)
{
	vector u = get_vector(a, b);
	vector v = get_vector(a, c);
	vector n = normal_vector(u, v);

	return n.x != 0 || n.y != 0 || n.z != 0;
}

bool is_side(std::vector<point> &points, point a, point b, point c)
{
	if(!is_plane(a, b, c))
		return false;
	vector u = get_vector(a, c);
	vector v = get_vector(a, b);
	plane p = plane(normal_vector(u, v), a);
	long double side = 0;


	for(auto x : points)
	{
		long double s = p.get_side(x);
		if(s == 0)
			continue;
		else if(side != 0 && s != side)
			return false;
		else
			side = s;

	}
	return true;
}

long double get_distance(point a, point b, point c, point x)
{
	vector u = get_vector(a, b);
	vector v = get_vector(a, c);
	vector n = normal_vector(u, v);
	return plane(n, a).get_distance(x);
}

long double side_distance(std::vector<point> &points, point x)
{
	unsigned long size = points.size();
	long double min = INT32_MAX;

	for(int i = 0; i < size; i++)
	{
		for(int j = i + 1; j < size; j++)
		{
			for(int k = j + 1; k < size; k++)
			{
				point a = points[i], b = points[j], c = points[k];
				if(is_side(points, a, b, c))
					min = std::min(min, get_distance(a, b, c, x));
			}
		}

	}
	return min;
}

int main()
{
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int x, y, z;
		int n_points;
		std::vector<point> points;
		std::cin>>x>>y>>z>>n_points;

		point runner(x, y, z);

		while(n_points--)
		{
			std::cin>>x>>y>>z;
			points.emplace_back(x, y, z);
		}

		std::cout<<std::fixed<<std::setprecision(6)<<side_distance(points, runner)<<"\n";
	}

	return 0;
}