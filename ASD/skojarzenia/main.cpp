#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>

#pragma GCC optimize("O3,no-stack-protector,loop-block,loop-interchange")
#pragma GCC target("sse4")

typedef std::pair<int, int> pair;

struct vertex
{
	bool used = false;
};

struct edge
{
	int f;
	int s;
	int l;

	edge(int f, int s, int l)
	{
		this->f = f;
		this->s = s;
		this->l = l;
	}
};



int get_id(char a, char b)
{
	a -= 'a';
	b -= 'a';

	return a * 26 + b;
}

int get_end_id(std::string &s)
{
	int len = s.length();

	return get_id(s[len - 2], s[len - 1]);
}

int get_beg_id(std::string &s)
{
	return get_id(s[0], s[1]);
}

class Dictionary
{
	std::vector<edge> edges;
	std::map<pair, int> map;
	int size;

	void add_edges(std::vector<std::string> &words)
	{
		std::set<int> set;
		for(auto &s : words)
		{
			set.insert(get_beg_id(s));
			set.insert(get_end_id(s));
			pair e(get_beg_id(s), get_end_id(s));
			if(map.count(e) == 0)
			{
				edges.emplace_back(get_beg_id(s), get_end_id(s), 0);
				map[e] = s.length();
			}
			else if(map[e] < s.length())
				map[e] = s.length();
		}
		size = set.size();
		for(auto &e : edges)
		{
			e.l = -map[pair(e.f, e.s)];
		}
	}

	bool relax(float *paths, float average) const
	{
		bool changed = false;
		for(auto e : edges)
		{
			if(paths[e.s] > paths[e.f] + e.l + average)
			{
				changed = true;
				paths[e.s] = paths[e.f] + e.l + average;
			}
		}
		return changed;
	}

public:


	explicit Dictionary(std::vector<std::string> &words, int s)
	{
		add_edges(words);
	}

	bool has_cycle(float average) const
	{
		float paths[26 * 26];
		for(int i = 0; i < 26 * 26; i++)
			paths[i] = 0;
		bool changed = true;
		int i;
		for(i = 0; (i < size + 2) && changed; i++)
			changed = relax(paths, average);
		return i >= size + 2;
	}
};


float get_average(const Dictionary &dict, float begin, float end)
{
	while(end - begin > 0.0001)
	{
		float s = (end + begin) / 2;

		if(dict.has_cycle(s))
		{
			begin = s;
		}
		else
			end = s;
	}
	return (begin + end) / 2;
}

int main()
{
	std::ios::sync_with_stdio(false);
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int n;
		std::cin>>n;
		std::vector<std::string> words;
		int min = 10000;
		int max = -1;
		for(int i = 0; i < n; i++)
		{
			std::string s;
			std::cin>>s;
			words.push_back(s);
			min = std::min(min, (int)s.length());
			max = std::max(max, (int)s.length());
		}
		Dictionary dict(words, n);
		std::cout<<std::fixed;
		if(dict.has_cycle(0))
			std::cout<<std::setprecision(4)<<get_average(dict, min, max)<<"\n";
		else
			std::cout<<"NIE\n";
	}

	return 0;
}