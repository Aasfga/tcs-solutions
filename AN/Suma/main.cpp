#include<iostream>

int main() {
    int n;
    double sum = 0;
    double c = 0;
    double value;
    double new_sum;

    std::cin>> n;
    while(n--) {
        std::cin>> value;
        value -= c;
        new_sum = sum + value;
        c = (new_sum - sum) - value;
        sum = new_sum;
    }
    std::cout.precision(5);
    std::cout<< sum<< "\n";
    return 0;
}
