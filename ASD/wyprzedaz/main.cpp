#include <iostream>
#include <queue>
#include <set>

typedef std::pair<int, int> pair;
struct shop
{
	static long id_counter;
	int money;
	pair open_time;
	long id;


	shop()
	{}

	shop(int a, int b, int m)
	{
		money = m;
		open_time.first = a;
		open_time.second = b;
		id = id_counter++;
	}
};

long shop::id_counter = 0;

typedef std::priority_queue<shop, std::vector<shop>, bool(*)(const shop&, const shop&)> Queue;
typedef std::set<shop, bool(*)(const shop&, const shop&)> Set;

bool comp_first(const shop &a, const shop &b)
{
	if(a.open_time.first == b.open_time.first)
		return a.id < b.id;
	else
		return a.open_time.first < b.open_time.first;
}

bool comp_second(const shop &a, const shop &b)
{
	return a.open_time.second > b.open_time.second;
}

bool comp_money(const shop &a, const shop &b)
{
	if(a.money == b.money)
		return a.id < b.id;
	else
		return a.money > b.money;
}

bool is_independent(Set set)
{
	Queue queue(comp_second);
	auto last = set.end();
	last--;
	for(auto iter = set.begin(); iter != last; iter++)
	{
		int crr_a = (*iter).open_time.first;
		iter++;
		int next_a = (*iter).open_time.first;
		iter--;
		queue.push(*iter);
		for(int j = crr_a; !queue.empty() && j < next_a; j++)
		{
			shop smallest = queue.top();
			if(smallest.open_time.second <= j)
				return false;
			else
				queue.pop();
		}
	}
	return true;
}

int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int n;
		std::cin>> n;
		Set set(comp_first);
		set.insert(shop(INT32_MAX - 1, INT32_MAX, 0));
		Set data(comp_money);
		for(int i = 0; i < n; i++)
		{
			int a, b, m;
			std::cin>> a>> b>> m;
			data.insert(shop(a, b, m));
		}

		for(auto x : data)
		{
			set.insert(x);
			if(!is_independent(set))
				set.erase(x);
		}

		long long sum = 0;
		for(auto x : set)
		 sum += x.money;
		std::cout<< sum<< "\n";

	}


	return 0;
}