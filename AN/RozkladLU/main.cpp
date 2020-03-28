#include<iostream>
#include<cmath>
class matrix;

typedef long double number;

typedef std::pair<matrix, matrix> decomposition;
typedef number* vector;


class matrix {
public:
    int size = 0;
    number **array = nullptr;
    int *swaps = nullptr;
    matrix(int size) {
        this->size = size;
        array = new number*[size];
        swaps = new int[size];
        for(int i = 0; i < size; i++) {
            array[i] = new number[size]{0};
        }
    }

    void pivoting() {
        for(int i = 0, max; i < size; i++) {
            max = i;
            for(int j = i; j < size; j++) {
                if(abs(array[j][i]) > abs(array[max][i])) {
                    max = j;
                }
            }
            std::swap(array[i], array[max]);
            swaps[i] = max;
        }
    }

    void pivot_results(vector results) {
        for(int i = 0; i < size; i++) {
            std::swap(results[i], results[swaps[i]]);
        }
    }

    matrix(matrix &&m) {
        std::swap(m.array, array);
        std::swap(m.size, size);
        std::swap(m.swaps, swaps);
    }

    number* operator[](int index) {
        if(index >= size) {
            std::cout<< "Index out of range\n";
            exit(1);
        }
        return array[index];
    }

    void calc_upper(matrix &lower, matrix &upper, int i, int j) {
        number sum = 0;
        for(int k = 0; k < i; k++) {
            sum += lower[i][k] * upper[k][j];
        }
        upper[i][j] = array[i][j] - sum;   
    }

    void calc_lower(matrix &lower, matrix &upper, int j, int i) {
        number sum = 0;
        for(int k = 0; k < i; k++) {
            sum += lower[j][k] * upper[k][i];
        }
        lower[j][i] = (array[j][i] - sum) / upper[i][i];
    }

    decomposition get_decomposition() {
        matrix lower(size), upper(size);
        for(int i = 0; i < size; i++) {
            lower[i][i] = 1;
        }
        for(int i = 0; i < size; i++) {
            for(int j = i; j < size; j++) {
                calc_upper(lower, upper, i, j);
            }

            if(i + 1 < size) {
                for(int j = 0; j < i + 1; j++) {
                    calc_lower(lower, upper, i + 1, j);
                }
            }
        }
        return decomposition(std::move(lower), std::move(upper));
    }

    ~matrix() {
        for(int i = 0; i < size; i++) {
            delete array[i];
        }
        if(array != nullptr) {
            delete [] array;
        }
        if(swaps != nullptr) {
            delete [] swaps;
        }

    }

    friend std::istream& operator>>(std::istream &stream, matrix &m) {
        for(int i = 0; i < m.size; i++) {
            for(int j = 0; j < m.size; j++) {
                stream>> m.array[i][j];
            }
        }
        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, const matrix &m) {
        stream.precision(10);
        for(int i = 0; i < m.size; i++) {
            for(int j = 0; j < m.size; j++) {
                stream<< std::fixed<< m.array[i][j]<< "   ";
            }
            stream<< "\n";
        }
        return stream;
    }
};

void print_result(vector results, int size) {
    std::cout.precision(10);
    for(int i = 0; i < size; i++) {
        std::cout << std::fixed<< results[i]<< " ";
    }
    std::cout<< "\n";
}

vector solve_lower(matrix &lower, vector results) {
    vector variables = new number[lower.size];

    for(int i = 0; i < lower.size; i++) {
        number sum = 0;
        for(int j = 0; j < i; j++) {
            sum += lower[i][j]*variables[j];
        }
        variables[i] = results[i] - sum;
    }
    return variables;
}

vector solve_upper(matrix &upper, vector results) {
    vector variables = new number[upper.size];

    for(int i = upper.size - 1; i >= 0; i--) {
        number sum = 0;
        for(int j = i + 1; j < upper.size; j++) {
            sum += upper[i][j] * variables[j];
        }
        variables[i] = (results[i] - sum) / upper[i][i];
    }
    return variables;
}

vector solve(matrix &lower, matrix &upper, vector results) {
    vector lower_result = solve_lower(lower, results);
    vector upper_result = solve_upper(upper, lower_result);
    delete [] lower_result;
    return upper_result;
}


int main() {
    int tasks;
    std::cin>> tasks;

    while(tasks--) {
        int size; 
        std::cin>> size;
        matrix m(size); 
        std::cin>> m;
        m.pivoting();
        int r;
        std::cin>> r;
        decomposition d = m.get_decomposition();
        vector results = new number[size];
        while(r--) {
            for(int i = 0; i < size; i++) {
                std::cin>> results[i];
            }
            m.pivot_results(results);
            vector variables = solve(d.first, d.second, results);
            print_result(variables, size);
            delete [] variables;
        }
        delete[] results;
    }

    return 0;
}