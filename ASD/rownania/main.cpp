#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>


class number
{
	long long mod;
	long long value;

public:

	number() = default;

	number(long long value, long long mod) : mod(mod), value(value%mod)
	{}

	number(const number &other) = default;

	number &operator=(const number &other) = default;

	number reverse() const
	{
		long long u = 1, v = 0, w = value;
		long long x = 0, y = 1, z = mod;
		long long q;
		while(w != 0)
		{
			if(w < z)
			{
				std::swap(u, x);
				std::swap(v, y);
				std::swap(w, z);
			}

			q = w/z;
			u -= q*x;
			v -= q*y;
			w -= q*z;
		}


		return number(x < 0 ? mod + x : x, mod);
	}

	number operator-() const
	{
		long long new_value = (mod - value)%mod;
		return number(new_value, mod);
	}

	number operator+(const number &other) const
	{
		assert(mod == other.mod);
		long long new_value = (value + other.value)%mod;
		return number(new_value, mod);
	}

	void operator+=(const number &other)
	{
		*this = *this + other;
	}

	number operator-(const number &other) const
	{
		return *this + (-other);
	}

	void operator-=(const number &other)
	{
		*this = *this - other;
	}

	number operator*(const number &other) const
	{
		assert(mod == other.mod);
		long long new_value = (value*other.value)%mod;
		return number(new_value, mod);
	}

	void operator*=(const number &other)
	{
		*this = *this*other;
	}

	number operator/(const number &other) const
	{
		assert(mod == other.mod);
		return *this*other.reverse();
	}

	void operator/=(const number &other)
	{
		assert(mod == other.mod);

		*this = *this/other;
	}

	friend std::ostream &operator<<(std::ostream &os, const number &number)
	{
		os<<number.value;
		return os;
	}

	bool operator==(const int x) const
	{
		return value == x;
	}

	bool operator!=(const int x) const
	{
		return !(*this == x);
	}

	bool operator==(const number &rhs) const
	{
		return mod == rhs.mod &&
		       value == rhs.value;
	}

	bool operator!=(const number &rhs) const
	{
		return !(rhs == *this);
	}
};

typedef number *equation;

class number_factory
{
	int prime;

public:
	explicit number_factory(int prime) : prime(prime)
	{}

	number operator()(int value)
	{
		return number(value, prime);
	}

	equation create_equation(int size)
	{
		auto eq = new number[size];
		for(int i = 0; i < size; i++)
			eq[i] = number(0, prime);
		return eq;
	}

	equation create_equation(long long *values, int size)
	{
		auto eq = new number[size];
		for(int i = 0; i < size; i++)
			eq[i] = number(values[i], prime);
		return eq;
	}
};


class linear_system
{
	number_factory factory;
	equation *equations;
	number *solution;
	bool *is_edgy;
	int *rows;
	int width;
	int height = 0;

	int zero_row = -1;

	int find_terminator(int row, int column)
	{
		for(int i = row; i < height; i++)
		{
			if(equations[i][column] != 0)
				return i;
		}
		return -1;
	}

	void eliminate(equation john_connor, equation terminator, int column)
	{
		number scalar = john_connor[column]/terminator[column];

		for(int i = column; i <= width; i++)
			john_connor[i] -= scalar*terminator[i];
//		print();
	}

	bool eliminate(int row, int column)
	{
		int terminator = find_terminator(row, column);
		if(terminator < 0)
			return false;
		std::swap(equations[row], equations[terminator]);
		for(int i = row + 1; i < height; i++)
			eliminate(equations[i], equations[row], column);
		return true;
	}

	bool is_correct()
	{
		bool result = true;
		for(int i = zero_row; i < height && result; i++)
			result = equations[i][width] == 0;
		return result;
	}

	bool many_solutions()
	{
		if(width > height)
			return true;

		bool result = false;
		for(int i = 0; i < width && !result; i++)
			result = equations[i][i] == 0;
		return result;
	}

	void set_rows()
	{
		int row = 0;
		int column = 0;

		while(column < width)
		{
			if(row + 1 < height && equations[row + 1][column] != 0)
				row++;
			rows[column] = row;
			column++;
		}
	}

	void set_edgy()
	{
		int column = 0;
		while(column < width && equations[0][column] == 0)
		{
			column++;
			is_edgy[column] = false;
		}
		if(column == width)
			return;

		is_edgy[column++] = true;

		while(column < width)
		{
			is_edgy[column] = rows[column - 1] != rows[column];
			column++;
		}
	}

	void set_solution()
	{
		set_rows();
		set_edgy();

		for(int v = width - 1; v >= 0; v--)
		{
			if(!is_edgy[v])
			{
				solution[v] = factory(0);
				continue;
			}
			int row = rows[v];
			number sum = factory(0);

			for(int i = v + 1; i < width; i++)
				sum += equations[row][i] * solution[i];
			number a = equations[row][v];
			number b = equations[row][width];
			solution[v] = (b - sum)/a;
		}
	}

public:
	linear_system(int width, int height, int prime) : width(width), height(height), factory(prime)
	{
		equations = new equation[height];
		solution = factory.create_equation(width);
		is_edgy = new bool[width];
		rows = new int[width];
	}

	void set_equation(long long *values, int row)
	{
		equations[row] = factory.create_equation(values, width + 1);
	}

	void print()
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j <= width; j++)
				std::cout<< std::setw(4)<< equations[i][j]<< " ";
			std::cout<< "\n";
		}
		std::cout<< "############\n";
	}

	void gaussian_elimination()
	{
		int row = 0;
		int column = 0;

		while(row < height && column < width)
		{
			if(eliminate(row, column))
				row++;
			column++;
		}
		zero_row = row;
	}

	void print_solution()
	{
		if(!is_correct())
		{
			std::cout<<"NIE\n";
			return;
		}

		if(many_solutions())
			std::cout<< "WIELE\n";
		else
			std::cout<< "TAK\n";
		set_solution();
		for(int i = 0; i < width; i++)
			std::cout<< solution[i]<< " ";
		std::cout<< "\n";
	}

	~linear_system()
	{
		for(int i = 0; i < height; i++)
			delete [] equations[i];
		delete [] equations;
		delete [] solution;
		delete [] is_edgy;
		delete [] rows;
	}
};



void read_row(long long *values, int width)
{
	for(int i = 0; i < width; i++)
		std::cin>> values[i];
}

int main()
{
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int height, width, prime;
		std::cin>>height>>width>>prime;
		linear_system matrix(width, height, prime);
		auto *values = new long long[width + 1];

		for(int i = 0; i < height; i++)
		{
			read_row(values, width + 1);
			matrix.set_equation(values, i);
		}

		delete [] values;

		matrix.gaussian_elimination();
//		matrix.print();
		matrix.print_solution();
	}
	return 0;
}