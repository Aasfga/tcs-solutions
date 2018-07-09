#include <iostream>
#include <set>
#include <vector>
#include <string.h>


typedef std::string string;
typedef std::vector<int> vector;

void init_hashes(string &text, int *hashes, int size, int p_size)
{
	for(int i = 0; i < text.size(); i++)
		hashes[i] = text[i] - 'a';
	for(int i = text.size(); i < size + p_size; i++)
		hashes[i] = 'z' - 'a' + 2;
}

void set_indexes(int size, int *indexes)
{
	for(int i = 0; i < size; i++)
		indexes[i] = i;
}

void count_sort(int *hashes, int *indexes, int size, int offset)
{
	auto *buckets = new vector[size + 27 + 3];

	for(int i = 0; i < size; i++)
	{
		int index = indexes[i];
		buckets[hashes[index + offset]].push_back(index);
	}

	auto cpy_indexes = new int[size];
	for(int i = 0, iter = 0; i < size + 27 + 1; i++)
	{
		for(auto index : buckets[i])
			cpy_indexes[iter++] = index;
	}
	memcpy(indexes, cpy_indexes, 4*size);

	delete [] buckets;
	delete [] cpy_indexes;
}


void swap_hashes(int *hashes, int *indexes, int size, int offset)
{
	int prev[] = {-1, -1};
	int crr[] = {0, 0};
	int hash = -1;

	auto cpy_hashes = new int[size];

	for(int i = 0; i < size; i++)
	{
		int index = indexes[i];
		crr[0] = hashes[index];
		crr[1] = hashes[index + offset];

		if(crr[0] != prev[0] || crr[1] != prev[1])
		{
			prev[0] = crr[0];
			prev[1] = crr[1];
			hash++;
		}
		cpy_hashes[index] = hash;
	}

	memcpy(hashes, cpy_hashes, 4*size);

	delete [] cpy_hashes;
}

int *create_kmr(string text, int p_size)
{
	int size = text.size();
	auto hashes = new int[size + p_size];
	init_hashes(text, hashes, size, p_size);
	int *indexes = new int[size];
	int b_size = 1;

	while(2 * b_size <= p_size)
	{
		set_indexes(size, indexes);
		count_sort(hashes, indexes, size, b_size);
		count_sort(hashes, indexes, size, 0);
		swap_hashes(hashes, indexes, size, b_size);
		b_size *= 2;
	}

	set_indexes(size, indexes);
	count_sort(hashes, indexes, size, p_size - b_size);
	count_sort(hashes, indexes, size, 0);
	swap_hashes(hashes, indexes, size, p_size - b_size);

	delete [] indexes;
	return hashes;
}




int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int p_size, t_size;
		string pattern, text;
		std::cin>> p_size>> pattern>> t_size>> text;
		int *hashes = create_kmr(pattern + pattern + text, p_size);
		std::set<int> set;

		for(int i = 0; i < p_size; i++)
			set.insert(hashes[i]);

		vector results;

		int start = 2*p_size;
		int end = 2*p_size + t_size;
		for(int i = start; i < end; i++)
		{
			if(set.find(hashes[i]) != set.end() && i + p_size <= end)
				results.push_back(i - start);
		}

		std::cout<< results.size()<< " ";
		for(auto x : results)
			std::cout<< x<< " ";
		std::cout<< "\n";

		delete [] hashes;
	}

	return 0;
}