#include <cmath>
#include <iostream>

class point
{
	long double x;
	long double y;


public:

	long double dist(const point &other) const
	{
		long double a = other.x - x;
		long double b = other.y - y;

		return sqrt(a*a + b*b);
	}

	friend std::istream &operator>>(std::istream &is, point &point)
	{
		is>>point.x>>point.y;
		return is;
	}
};

void print_equation(int n)
{
	std::cout<<"max: ";

	for(int i = 0; i < n - 1; i++)
		printf("x%i + ", i);
	printf("x%i;\n", n - 1);
}

void print_bounds(point *points, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
			printf("x%i + x%i <= %.6Lf;\n", i, j, points[i].dist(points[j]));
	}

}

int main()
{
	int n;
	std::cin>>n;
	auto *points = new point[n];
	for(int i = 0; i < n; ++i)
		std::cin>>points[i];
	print_equation(n);
	print_bounds(points, n);
	delete [] points;

	return 0;
}

