#include <iostream>
#include <random>

#define integer unsigned long long int

integer power(integer a, integer x, integer mod) {
	integer res = 1;
	while(x) {
		if(x & 1u)
			res = (res*a)%mod;
		a = (a*a)%mod;
		x >>= 1u;
	}
	return res;
}

integer get_non_residue(integer p) {
	std::random_device device;
	std::mt19937_64 generator(device());
	std::uniform_int_distribution<integer> distribution(1, p - 1);

	integer z = distribution(generator);
	while(power(z, (p - 1)/2, p) != p - 1)
		z = distribution(generator);
	return z;
}

integer square_root(integer a, integer p) {
	if(p == 2)
		return 1;
	if(power(a, (p - 1)/2, p) != 1)
		return 0;
	if((p + 1)%4 == 0)
		return power(a, (p + 1)/4, p);
	integer q = (p - 1)/2;
	integer t = p - 1;
	integer z = get_non_residue(p);

	while(q%2 == 0) {
		q /= 2;
		t /= 2;
		if((power(a, q, p)*power(z, t, p))%p != 1)
			t = t + (p - 1)/2;
	}
	return (power(a, (q + 1)/2, p)*power(z, t/2, p))%p;
}


int main() {
	int tasks;
	std::cin>>tasks;

	while(tasks--) {
		integer a, p;
		std::cin>>a>>p;
		integer s = square_root(a, p);
		if(s)
			std::cout<<s<<"\n";
		else
			std::cout<<"NIE\n";
	}
	return 0;
}
