#include <iostream>
#include <algorithm>
#define MODULO 1000000007


void set_kmp(const std::string &pattern, int *kmp)
{
	kmp[0] = -1;
	int i = -1;
	unsigned long size = pattern.size() + 1;

	for(int j = 1; j < size; j++)
	{
		while(i >= 0 && pattern[j - 1] != pattern[i])
			i = kmp[i];
		kmp[j] = ++i;
	}
}

void set_state(const std::string &pattern, int *state, int *kmp, char x)
{
	unsigned long size = pattern.size() + 1;
	int i;
	state[0] = (pattern[0] == x ? 1 : 0);
	for(int j = 1; j < size; j++)
	{
		i = j;
		while(i >= 0 && x != pattern[i])
			i = kmp[i];
		state[j] = i + 1;
	}
}

void set_states(const std::string &pattern, int **states, int *kmp)
{
	set_state(pattern, states[0], kmp, 'a');
	set_state(pattern, states[1], kmp, 'b');
}


int count_words(const std::string &pattern, int word_size)
{
	int **states = new int*[2];
	int **result = new int*[2];
	states[0] = new int[pattern.size() + 1];
	states[1] = new int[pattern.size() + 1];
	result[0] = new int[pattern.size() + 1];
	result[1] = new int[pattern.size() + 1];
	auto *kmp = new int[pattern.size() + 1];
	set_kmp(pattern, kmp);
	set_states(pattern, states, kmp);
	std::fill_n(result[0], pattern.size() + 1, 0);
	int row = 0;
	result[row][0] = 1;

	for(int i = 0; i <= word_size; i++)
	{
		std::fill_n(result[1 - row], pattern.size() + 1, 0);
		for(int j = 0; j < pattern.size(); j++)
		{
			int state_a = states[0][j];
			int state_b = states[1][j];
			result[1 - row][state_a] += result[row][j];
			result[1 - row][state_a] %= MODULO;
			result[1 - row][state_b] += result[row][j];
			result[1 - row][state_b] %= MODULO;
		}
		row = 1 - row;
	}

	int sum = 0;
	for(int i = 0; i < pattern.size(); i++)
	{
		sum += result[1 - row][i];
		sum %= MODULO;
	}



	delete [] result[0];
	delete [] result[1];
	delete [] states[0];
	delete [] states[1];
	delete [] states;
	delete [] result;
	delete [] kmp;
	return sum;
}

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int length;
		std::string pattern;
		std::cin>> length>> pattern;

		std::cout<< count_words(pattern, length)<< "\n";
	}
	return 0;
}