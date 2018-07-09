#include <iostream>
#include <vector>
#include <set>


class Cell
{

	bool _in_cache = false;
	std::vector<int> when_is_needed;
	int i = 0;

public:

	void add(int x)
	{
		when_is_needed.push_back(x);
	}

	int give_first()
	{
		return when_is_needed[i];
	}

	void pop_first()
	{
		i++;
	}

	void put()
	{
		_in_cache = true;
	}

	void remove()
	{
		_in_cache = false;
	}

	bool in_cache()
	{
		return _in_cache;
	}

	bool operator<(const Cell &rhs) const
	{
		return when_is_needed[i] < rhs.when_is_needed[rhs.i];
	}

	bool operator>(const Cell &rhs) const
	{
		return rhs < *this;
	}

	bool operator<=(const Cell &rhs) const
	{
		return !(rhs < *this);
	}

	bool operator>=(const Cell &rhs) const
	{
		return !(*this < rhs);
	}
};

typedef std::vector<Cell> Cells;



class Cache
{
	typedef std::pair<int, int> pair;
	int size;
	Cells *cells;
	long copy_counter = 0;
	std::set<pair> cache;

public:
	Cache(int size, Cells *cells): size(size), cells(cells)
	{}

	long give_copy_counter()
	{
		return copy_counter;
	}

	void put_cell(int c)
	{
		if((*cells)[c].in_cache())
		{
			(*cells)[c].pop_first();
			cache.erase(*cache.begin());
			cache.insert(pair((*cells)[c].give_first(), c));
		}
		else if(cache.size() < size)
		{
			copy_counter++;
			(*cells)[c].put();
			(*cells)[c].pop_first();
			cache.insert(pair((*cells)[c].give_first(), c));
			return;
		}
		else
		{
			copy_counter++;
			(*cells)[c].pop_first();
			(*cells)[c].put();
			pair last = (*cache.rbegin());
			cache.insert(pair((*cells)[c].give_first(), c));
			(*cells)[last.second].remove();
			cache.erase(last);
		}

	}

};


int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int copy_counter = 0;
		int n,k,m;
		std::cin>> k>> n>> m;
		std::vector<Cell> cells(n, Cell());
		auto *needed_cells = new int[m];
		Cache cache(k, &cells);
		for(int i = 0; i < m; i++)
		{
			int x;
			std::cin>> x;
			needed_cells[i] = x - 1;
			cells[x - 1].add(i);
		}
		for(int i = 0; i < n; i++)
			cells[i].add(m + 1 + i);


		for(int i = 0; i < m; i++)
		{
			cache.put_cell(needed_cells[i]);
		}

		std::cout<< cache.give_copy_counter()<< "\n";
		delete [] needed_cells;
	}

	return 0;
}