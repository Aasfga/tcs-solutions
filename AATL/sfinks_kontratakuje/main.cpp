#include <iostream>
#include <cmath>
#include <random>
#include <list>
#include <algorithm>
#include <tuple>
#include <tgmath.h>

typedef unsigned long long int integer;
typedef std::pair<integer, integer> pair;
typedef std::list<integer> list;
std::random_device *device;
std::mt19937_64 *generator;

integer abs_sub(integer a, integer b) {
	return a < b ? b - a : a - b;
}

integer log_floor(integer x) {
	return 63u - __builtin_clzl(x);
}

integer log_ceil(integer x) {
	return log_floor(x - 1) + 1;
}


integer mul(integer a, integer b, integer mod) {
	integer res = 0;
	while(b) {
		if(b & 1u)
			res = (res + a)%mod;
		b >>= 1u;
		a = (2*a)%mod;
	}
	return res;
}

integer power(integer a, integer x, integer mod) {
	integer res = 1u;
	while(x) {
		if(x & 1u)
			res = mul(res, a, mod);
		a = mul(a, a, mod);
		x >>= 1u;
	}
	return res;
}

pair two_power_form(integer d) {
	integer r = 0;
	while(d%2 == 0) {
		d /= 2;
		r++;
	}
	return {r, d};
}

bool iteration(integer n, integer r, integer d) {
	std::uniform_int_distribution<integer> distribution(2, n - 2);

	integer a = distribution(*generator);
	integer x = power(a, d, n);
	if(x == 1 || x == n - 1)
		return true;
	bool result = false;
	for(int i = 1; i < r && !result; i++) {
		x = mul(x, x, n);
		result = (x == n - 1);
	}
	return result;
}

bool is_prime(integer n, int k = 20) {
	if(n%2 == 0)
		return n == 2;
	else if(n == 3)
		return true;
	integer r, d;
	std::tie(r, d) = two_power_form(n - 1);
	bool result = true;
	for(int i = 0; i < k && result; i++)
		result = iteration(n, r, d);
	return result;
}

pair power_form(integer n) {
	integer k = log_ceil(n) + 1;
	integer a = 0;
	while(pow(a, k) != n) {
		k--;
		a = (integer) roundl(powl(n, 1.0l/k));
	}
	return {a, k};
}

integer gcd(integer a, integer b) {
	while(b != 0) {
		if(a < b)
			std::swap(a, b);
		else
			a %= b;
		b += 0;
	}
	return a;
}

integer rho_function(integer x, integer c, integer n) {
	return (mul(x, x, n) + c)%n;
}

integer rho_pollard(integer n) {
	std::uniform_int_distribution<integer> distribution(1, n - 3);
	integer x, y, d = n;
	while(d == n) {
		integer c = distribution(*generator);
		x = 2;
		y = 2;
		d = 1;

		while(d == 1) {
			x = rho_function(x, c, n);
			y = rho_function(y, c, n);
			y = rho_function(y, c, n);
			d = gcd(abs_sub(x, y), n);
		}
	}

	return d;
}

void append(list &dest, list &src) {
	dest.insert(dest.end(), src.begin(), src.end());
}

list factorization(integer n) {
	list factors;
	integer a, k;
	std::tie(a, k) = power_form(n);
	if(is_prime(a)) {
		factors.push_front(a);
	}
	else if(a%2 == 0) {
		factors = factorization(a/2ull);
		factors.push_front(2);
	}
	else {
		integer p = rho_pollard(a);
		integer q = a/p;
		list factors_p = factorization(p);
		list factors_q = factorization(q);
		append(factors, factors_p);
		append(factors, factors_q);
	}
	list result;
	for(integer i = 0; i < k; i++) {
		append(result, factors);
	}

	return result;
}

int main() {
	device = new std::random_device;
	generator = new std::mt19937_64((*device)());
	int tasks;
	list factors;
	std::cin>>tasks;
	while(tasks--) {
		integer n;
		std::cin>>n;
		factors = factorization(n);
		factors.sort();
		for(auto x : factors)
			std::cout<<x<<" ";
		std::cout<<"\n";
	}
	return 0;
}