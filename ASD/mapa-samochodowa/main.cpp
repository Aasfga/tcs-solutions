#include <iostream>
#include <map>


typedef std::map<std::string, int> map;
typedef std::map<int, std::string> rev_map;

int** create_matrix(int n)
{
	auto **matrix = new int*[n];
	for(int i = 0; i < n; i++)
		matrix[i] = new int[n];

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			matrix[i][j] = (i == j ? 0 : 10000000);
	}

	return matrix;
}


void calc_matrix(int **matrix, int n)
{
	for(int k = 0; k < n; k++)
	{
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				int new_dist = matrix[i][k] + matrix[k][j];
				if(new_dist < matrix[i][j])
					matrix[i][j] = new_dist;
			}
		}
	}
}

void print_label(rev_map &rev, int n)
{
	std::cout<< std::string(12, ' ');
	for(int i = 0; i < n; i++)
	{
		std::cout.width(12);
		std::cout<< std::right<< rev[i];
	}
	std::cout<< "\n";
}

void print_row(std::string &name, int *row, int n)
{
	std::cout.width(12);
	std::cout<< std::right<< name;
	for(int i = 0; i < n; i++)
	{
		std::cout.width(12);
		std::cout<< std::right<< row[i];
	}
	std::cout<< "\n";
}

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		map names;
		rev_map rev_names;
		int v_size, e_size;
		std::cin>> v_size;
		int **matrix = create_matrix(v_size);
		for(int i = 0; i < v_size; i++)
		{
			std::string name;
			std::cin>> name;
			names[name] = i;
			rev_names[i] = name;
		}
		std::cin>> e_size;
		while(e_size--)
		{
			std::string u, v;
			int cost;
			std::cin>> u>> v>> cost;
			matrix[names[u]][names[v]] = cost;
			matrix[names[v]][names[u]] = cost;
		}
		calc_matrix(matrix, v_size);
		print_label(rev_names, v_size);
		for(int i = 0; i < v_size; i++)
			print_row(rev_names[i], matrix[i], v_size);

		for(int i = 0; i < v_size; i++)
			delete [] matrix[i];
		delete [] matrix;
	}
	return 0;
}