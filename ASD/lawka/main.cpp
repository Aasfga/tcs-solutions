#include <iostream>
#include <algorithm>
#include <string>
#include <vector>


struct Member
{
	int begin = 0;
	int end = 0;
	long photos = 0;
};

bool compare_members(const Member &first, const Member &second)
{
	return first.end < second.end;
}

typedef std::vector<Member> Members;
typedef std::pair<int, long> pair;

Member read_member()
{
	Member m;
	std::cin>> m.begin>> m.end>> m.photos;

	return m;
}

std::string to_string(const Member &m)
{
	return std::to_string(m.end);
}

bool compare_pair(const pair &p, const pair &q)
{
	return p.first < q.first;
}


int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int n;
		std::cin>> n;
		Members members(n);
		std::vector<pair> prefix_sums;
		prefix_sums.emplace_back(-1, 0);
		for(int i = 0; i < n; i++)
			members[i] = read_member();
		std::sort(members.begin(), members.end(), compare_members);

		for(Member m : members)
		{
			if(prefix_sums.back().first != m.end)
				prefix_sums.emplace_back(m.end, prefix_sums.back().second);
			auto begin = std::lower_bound(prefix_sums.begin(), prefix_sums.end(), pair(m.begin, 0), compare_pair);
			begin--;
			long k = m.photos - (prefix_sums.back().second - begin->second);
			if(k > 0)
				prefix_sums.back().second += k;
		}

		std::cout<< prefix_sums.back().second<< "\n";
	}
	return 0;
}