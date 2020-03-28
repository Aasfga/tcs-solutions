#include <iostream>
#include <cmath>

typedef long double number;
#define STEPS 10000

class Function {
	number array[6]{};
	int size = 0;
	number left = 0;
	number right = 0;


	Function() = default;

public:

	number operator()(number x) {
		number value = 0;

		for(int i = size - 1; i >= 0; i--) {
			value *= x;
			value += array[i];
		}

		return value;
	}

	number left_bound() {
		return left;
	}

	number right_bound() {
		return right;
	}

	static Function read_function() {
		Function fun;
		std::cin>>fun.size>>fun.left>>fun.right;
		for(int i = 0; i < fun.size; i++) {
			std::cin>>fun.array[i];
		}

		return fun;
	}
};

number calc_curve(Function f) {
	number step = (f.right_bound() - f.left_bound())/STEPS;
	number curve = 0;

	for(int i = 0; i < STEPS; i++) {
		number x = f.left_bound() + (f.right_bound() - f.left_bound())/STEPS*i;
		number height = f(x) - f(x + step);
		curve += sqrtl(height*height + step*step);
	}
	return curve;
}


int main() {
	int tasks;
	std::cin>>tasks;

	std::cout.precision(10);
	while(tasks--) {
		Function f = Function::read_function();
		std::cout<<std::fixed<<calc_curve(f)<<"\n";
	}
	return 0;
}