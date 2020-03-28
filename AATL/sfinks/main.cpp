#include <iostream>
#include <random>
#include <tuple>

#define integer unsigned long long int
#define integer_max ((integer)0-1)
typedef std::pair<integer, integer> pair;

integer mul(integer a, integer b, integer mod=integer_max) {
    integer res = 0;
    while(b) {
        if(b & 1)
            res = (res + a) % mod;
        b >>= 1;
        a = (2*a) % mod;
    }
    return res;
}

integer power(integer a, integer x, integer mod=integer_max) {
    integer res = 1;
    while(x) {
        if(x & 1)
            res = mul(res, a, mod);
        a = mul(a, a, mod);
        x >>= 1;
    }
    return res;
}

pair power_form(integer d) {
    integer r = 0;
    while(d % 2 == 0) {
        d /= 2;
        r++;
    }
    return pair(r, d);
}

bool iteration(integer n, integer r, integer d) {
    std::random_device device;
    std::mt19937_64 generator(device());
    std::uniform_int_distribution<integer> distribution(2, n - 2);

    integer a = distribution(generator);
    integer x = power(a, d, n);
    if(x == 1 || x == n-1)
        return true;
    bool result = false;
    for(int i = 1; i < r && !result; i++) {
        x = mul(x, x, n);
        result = (x == n - 1);
    }
    return result;
}

bool is_prime(integer n, int k = 20) {
    if(n % 2 == 0)
        return n == 2;
    else if(n == 3)
        return true;
    integer r, d;
    std::tie(r, d) = power_form(n - 1);
    bool result = true;
    for(int i = 0; i < k && result; i++)
        result = iteration(n, r, d);
    return result;
}

int main() {
    int numbers;
    std::cin>> numbers;
    while(numbers--) {
        unsigned long long n;
        std::cin>> n;
        std::cout<<(is_prime(n) ? "TAK" : "NIE")<< "\n";
    }

    return 0;
}