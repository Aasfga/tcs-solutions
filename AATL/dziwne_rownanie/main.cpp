#include <iostream>
#include <unordered_map>
#include <random>
#include <vector>

typedef unsigned long long int number;
std::vector<number> divisors;
number prime, _m, _a;

number sqrt(number x) {
	number result = 0;
	while(result*result < x)
		result++;
	return result;
}

number power(number a, number x) {
	number res = 1;

	while(x) {
		if(x & 1u)
			res = (res*a)%prime;
		a = (a*a)%prime;
		x /= 2;
	}
	return res;
}

number inverse(number x) {
	return power(x, prime - 2);
}

number equation(number n) {
	return (power(n, n) + power(n, _m))%prime;
}

bool check(number n) {
	return equation(n) == _a;
}

number brute_solver() {
	int i = 1;
	number max_i = prime*prime;
	while(i < max_i && !check(i))
		i++;
	return i == max_i ? 0 : i;
}

bool is_good_candidate(number g) {
	return power(g, _m) != _a;
}

bool is_generator(number g) {
	for(auto x : divisors) {
		if(power(g, x) == 1)
			return false;
	}
	return true;
}

number find_generator() {
	static std::random_device device;
	static std::mt19937_64 rand_gen(device());
	std::uniform_int_distribution<number> distribution(2, prime - 1);
	number g = distribution(rand_gen);
	while(!is_good_candidate(g) || !is_generator(g))
		g = distribution(rand_gen);
	return g;
}

number logarithm(number g, number c) {
	number m = sqrt(prime);
	std::unordered_map<number, number> map;

	number small_step = 1;
	for(number j = 0; j < m; j++) {
		map[small_step] = j;
		small_step = (small_step*g)%prime;
	}
	number giant_step = inverse(power(g, m));
	number crr_step = c;
	for(number i = 0; i < m; i++) {
		if(map.find(crr_step) != map.end())
			return i*m + map[crr_step];
		crr_step = (crr_step * giant_step) % prime;
	}
	return 0;
}

number random_solver() {
	number g = find_generator();
	number g_m = power(g, _m);
	number div = inverse(power(g, g));
	number c = _a > g_m ? _a - g_m : _a + prime - g_m;
	c = (c*div)%prime;
	number k = logarithm(g, c);
	return g + k*prime;
}

void add_divisors() {
	number x = prime - 1;
	for(int i = 2; i*i <= x; i++) {
		if(x%i == 0) {
			divisors.push_back(i);
			divisors.push_back(x/i);
		}
	}
}

int main() {
	int tasks;
	std::cin>>tasks;

	while(tasks--) {
		std::cin>>prime>>_a>>_m;
		add_divisors();
		number answer = prime < 200 ? brute_solver() : random_solver();
		if(answer) {
			std::cout<<"TAK "<<answer<<"\n";
		}
		else {
			std::cout<<"NIE\n";
		}

		divisors.clear();
	}
}
