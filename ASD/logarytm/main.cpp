#include <iostream>
#include <unordered_map>


typedef std::unordered_map<long, long> unordered_map;


long _sqrt(long x)
{
	long result = 0;

	while(result*result < x)
		result++;

	return result;
}

long pow_mod(long a, long x, long p)
{
	long result = 1;

	while(x--)
		result = (result * a) % p;

	return result;
}

long logarithm(long a, long b, long p)
{
	if(b == 0)
		return 1;
	long r = _sqrt(p);
	unordered_map map;

	long small_step = b;
	for(int j = 0; j <= r; j++)
	{
		map[small_step] = j;
		small_step = (small_step * a) % p;
	}

	long step = pow_mod(a, r, p);
	long crr_step = step;
	for(int i = 1; i <= r; i++)
	{
		if(map.find(crr_step) != map.end())
			return i * r - map[crr_step];
		crr_step = (crr_step * step) % p;
	}

	return -1;
}


int main()
{
	int tasks;
	std::cin>> tasks;

	while(tasks--)
	{
		int a, b, p;
		std::cin>> a>> b>> p;
		std::cout<<logarithm(a, b, p) << "\n";
	}
	return 0;
}