#include <iostream>
#include <cmath>
#include <unordered_map>
#include <functional>

#define number unsigned long long int
#define ERROR ((number) 0 - 1)
number mod = 0;
number upper_bound;
number a;
number b;


struct vector {
    number values[2] = {0, 0};

    vector() = default;

    vector(number v0, number v1) {
        values[0] = v0;
        values[1] = v1;
    }

    number hash() const {
        number x = values[0];
        number y = values[1];
        return std::hash<number>{}(x) ^ std::hash<number>{}(y);
    }

    number &operator[](int index) {
        return values[index];
    }

    bool operator==(const vector &rhs) const {
        return values[0] == rhs.values[0] && values[1] == rhs.values[1];
    }

    bool operator!=(const vector &rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const vector &vector) {
        os<<"{";
        os<<vector.values[0]<<","<<vector.values[1];
        os<<"}";
        return os;
    }
};

struct matrix {
    number values[2][2] = {{0, 0},
                           {0, 0}};

    matrix() = default;

    matrix(int v0, int v1, int v2, int v3) {
        values[0][0] = v0;
        values[0][1] = v1;
        values[1][0] = v2;
        values[1][1] = v3;
    }

    matrix &operator=(matrix other) {
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                values[i][j] = other[i][j];
            }
        }
        return *this;
    }

    number *operator[](int index) {
        return values[index];
    }

    matrix operator*(matrix &other) {
        matrix result;
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                for(int k = 0; k < 2; k++) {
                    result[i][j] += values[i][k]*other.values[k][j];
                    result[i][j] %= mod;
                }
            }
        }
        return result;
    }

    matrix operator+(matrix &other) {
        matrix result;
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                result[i][j] = values[i][j] + other[i][j];
                result[i][j] %= mod;
            }
        }
        return result;
    }

    matrix operator^(number x) {
        matrix acc = *this;
        matrix result(1, 0, 0, 1);

        while(x) {
            if(x & 1u)
                result = result*acc;
            acc = acc*acc;
            x /= 2;
        }
        return result;
    }

    vector operator*(vector &v) {
        vector result;
        for(int i = 0; i < 2; i++) {
            result[i] = values[i][0]*v[0] + values[i][1]*v[1];
            result[i] %= mod;
        }
        return result;
    }

    friend std::ostream &operator<<(std::ostream &os, const matrix &matrix) {
        os<<"{{";
        os<<matrix.values[0][0]<<","<<matrix.values[0][1];
        os<<"}, ";
        os<<"{";
        os<<matrix.values[1][0]<<","<<matrix.values[1][1];
        os<<"}}";
        return os;
    }
};

struct vector_hash {
    size_t operator()(const vector &v) const {
        return v.hash();
    }
};

number find_solution() {
    number m = ceil(sqrt(upper_bound));
    matrix fib_matrix(1, 1, 1, 0);
    matrix inv_matrix(0, 1, 1, mod - 1);
    std::unordered_map<vector, number, vector_hash> map;
    vector small_step(1, 0);
    vector crr_step(b, a);

    for(number i = 0; i <= m; i++) {
        if(small_step == crr_step)
            return i;
        map[small_step] = i;
        small_step = fib_matrix*small_step;
    }

    matrix giant_step = (inv_matrix ^ m);
    for(number i = 0; i <= m; i++) {
        if(map.find(crr_step) != map.end())
            return i*m + map[crr_step];
        crr_step = giant_step*crr_step;
    }
    return ERROR;
}

int main() {
    int tasks;
    std::cin>>tasks;

    while(tasks--) {
        std::cin>>mod>>a>>b>>upper_bound;
        number x = find_solution();
        if(x == ERROR || x > upper_bound) {
            std::cout<<"-\n";
        } else {
            std::cout<<x<<"\n";
        }
    }
}