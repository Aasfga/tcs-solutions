#include<iostream>

int main() {
    int n;
    long double sum = 0;
    long double c = 0;
    long double value;
    long double new_sum;

    std::cin>> n;
    while(n--) {
        std::cin>> value;
        value -= c;
        new_sum = sum + value;
        c = (new_sum - sum) - value;
        sum = new_sum;
    }
    std::cout.precision(10);
    std::cout<< std::fixed<< sum<< "\n";
    return 0;
}
