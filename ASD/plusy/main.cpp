#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <limits>

#define HORIZONTAL 0
#define VERTICAL 1


struct segment
{
	long long x;
	long long y;
	long long l;
	int type;

	segment() = default;

	segment(long long int x, long long int y, long long int l, int type) : x(x), y(y), l(l), type(type)
	{}

	bool operator==(const segment &rhs) const
	{
		return x == rhs.x &&
		       y == rhs.y &&
		       l == rhs.l &&
		       type == rhs.type;
	}

	bool is_horizontal() const
	{
		return type == HORIZONTAL;
	}

	bool is_vertical()
	{
		return type == VERTICAL;
	}
};

struct segment_comparator
{
	bool operator()(const segment &first, const segment &second) const
	{
		if(first.y != second.y)
			return first.y < second.y;
		if(first.x != second.x)
			return first.x < second.x;
		if(first.l != second.l)
			return first.l < second.l;
		return first.type < second.type;
	}
};

typedef std::pair<long double, segment> event;


bool is_erase(long double x, const segment &s)
{
	return s.x + s.l == x;
}


bool is_crossing(std::vector<segment> segments, long long d)
{
	std::vector<event> events;
	for(auto s : segments)
	{
		if(2*d > s.l)
			continue;
		s.l -= 2*d;
		if(s.is_horizontal())
		{
			s.x += d - 0.1;
			s.l += 0.1;
			events.emplace_back(s.x, s);
			events.emplace_back(s.x + s.l, s);
		}
		else
		{
			s.y += d;
			events.emplace_back(s.x, s);
		}
	}

	std::sort(events.begin(), events.end(), [](const event &f, const event &s)
	{
		return f.first < s.first;
//		if(f.first != s.first)
//			return f.first < s.first;
//		if(f.second.is_horizontal() == s.second.is_horizontal())
//			return f.second.y < s.second.y;
//
//		const event h = f.second.is_horizontal() ? f : s;
//
//		return is_erase(h.first, h.second) != (f == h);
	});

	std::set<segment, segment_comparator> broom;
	broom.emplace(0, INT32_MAX, INT32_MAX, HORIZONTAL);
	broom.emplace(0, INT32_MIN, INT32_MAX, HORIZONTAL);

	for(auto e : events)
	{
		long double x = e.first;
		segment s = e.second;
		if(s.is_horizontal() && is_erase(x, s))
			broom.erase(s);
		else if(s.is_horizontal())
			broom.insert(s);
		else
		{
			long long start = s.y;
			long long end = s.y + s.l;
			auto iter = broom.lower_bound(segment(0, start, 0, VERTICAL));
			if(iter->y <= end)
				return true;
		}
	}
	return false;
}


long long max_length(std::vector<segment> &segments)
{
	long long max = -1;

	for(auto s : segments)
		max = std::max(max, s.l);
	return max;
}

long long get_plus(std::vector<segment> &segments)
{
	long long b = max_length(segments);
	long long a = 0;

	while(a != b)
	{
		long long s = (a + b + 1)/2;

		if(is_crossing(segments, s))
			a = s;
		else
			b = s - 1;
	}

	return a;
}

int main()
{
	int tasks;
	std::cin>>tasks;

	while(tasks--)
	{
		int h, v;
		int x, y, l;
		std::cin>>v>>h;
		std::vector<segment> segments;

		while(v--)
		{
			std::cin>> x>> y>> l;
			segments.emplace_back(x, y, l, VERTICAL);
		}
		while(h--)
		{
			std::cin>> x>> y>> l;
			segments.emplace_back(x, y, l, HORIZONTAL);
		}

		std::cout<< get_plus(segments)<< "\n";


	}
	return 0;
}