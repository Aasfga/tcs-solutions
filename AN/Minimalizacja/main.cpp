#include <iostream>
#include <cmath>


typedef long double number;
typedef std::pair<number, number> pair;


number A = 0;
number B = 0;


pair calc_gradient(number x, number y) {
	number sinus = sin(x/2 - y);
	return {
			(number) 2*x + A - sinus/2,
			(number) 2*y + B + sinus
	};
}

pair calc_gradient(pair a) {
	return calc_gradient(a.first, a.second);
}

pair sub(pair a, pair b) {
	return {
			a.first - b.first,
			a.second - b.second
	};
}

number mul(pair a, pair b) {
	return a.first*b.first + a.second*b.second;
}

number calc_sigma(pair prev, pair crr) {
	pair p = sub(crr, prev);
	pair g = sub(calc_gradient(crr), calc_gradient(prev));
	return mul(p, g)/mul(g, g);
}

number error(pair a, pair b) {
	pair p = sub(a, b);
	return mul(p, p);
}


#define PREV (points[1-i])
#define CRR (points[i])
#define SWAP i = 1 - i;

pair find_minimum() {
	pair points[2]{{1, 1}, {0, 0}};
	int i = 1;

	while(error(PREV, CRR) > 0.00000001) {
		number sigma = calc_sigma(PREV, CRR);
		pair grad = calc_gradient(CRR);
		PREV.first = CRR.first - sigma * grad.first;
		PREV.second = CRR.second - sigma * grad.second;
		SWAP
	}

	return CRR;
}


int main() {
	int tasks;
	std::cin>>tasks;

//	std::cout.precision(6);
	while(tasks--) {
		std::cin>>A>>B;
		pair minimum = find_minimum();
		std::cout<<minimum.first<<" "<<minimum.second<<"\n";
	}

	return 0;
}