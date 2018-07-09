//
// Created by Dominik Grybos on 27.11.2017.
//

#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <climits>

#define MAXN 5005
using namespace std;
unsigned long long serial_gen = 1000;

struct Sale
{
	long long start;
	long long end;
	long long cost;
	long long serial;

	Sale() : start(0), end(0), cost(0)
	{}

	Sale(long long l, long long r, long long c) : start(l), end(r), cost(c), serial(++serial_gen)
	{}
};

struct cost_comp
{
	inline bool operator()(const Sale &s1, const Sale &s2)
	{
		if(s1.cost == s2.cost)
			return s1.serial < s2.serial;
		return s1.cost > s2.cost;
	}
};

struct start_comp
{
	inline bool operator()(const Sale &s1, const Sale &s2)
	{
		if(s1.start == s2.start)
			return s1.serial < s2.serial;
		return s1.start < s2.start;
	}
};

struct end_comp
{
	inline bool operator()(const Sale &s1, const Sale &s2)
	{
		return s1.end > s2.end;
	}
};

long long t, n, l, r, c;
Sale sales[MAXN];

bool independent(set<Sale, start_comp> S)
{
	Sale *x, *x_next;
	int current;
	priority_queue<Sale, vector<Sale>, end_comp> Q;
	vector<Sale> values;
	for(auto v : S)
	{
		values.push_back(v);
	}
	for(int i = 0; i < values.size() - 1; ++i)
	{
//std::cout << "::" << values[i].cost << std::endl;
		x = &values[i], x_next = &values[i + 1];
		Q.push(values[i]);
		current = x->start;
		while(!Q.empty() && current < x_next->start)
		{
			if(Q.top().end <= current)
				return false;
			else
				Q.pop();
			++current;
		}
	}
	return true;
}

long long compute(int n, Sale sales[])
{
	long long sum = 0;
	set<Sale, start_comp> S;
	S.insert(Sale(LLONG_MAX - 1, LLONG_MAX, 0));
//sort(sales, sales + n, cost_comp());
	set<Sale, cost_comp> elems;
	for(int i = 0; i < n; ++i)
	{
		elems.insert(sales[i]);
	}
	for(auto elem : elems)
	{
//std::cout << sales[i].cost << std::endl;
		S.insert(elem);
		if(independent(S))
		{
//std::cout << "INDEPENDENT" << std::endl;
		}
		else
		{
			S.erase(elem);
		}
	}
	for(auto current : S)
	{
		sum += current.cost;
	}
	return sum;
}

int main()
{
	cin>>t;
	while(cin>>n)
	{
		serial_gen = 0;
		for(int i = 0; i < n; ++i)
		{
			cin>>l>>r>>c;
			sales[i] = Sale(l, r, c);
		}
		cout<<compute(n, sales)<<endl;
	}
}