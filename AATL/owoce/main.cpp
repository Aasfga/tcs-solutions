#include <iostream>
#include <gmp.h>
#include <gmpxx.h>

#define number mpq_class

struct elliptic_curve {
    number k; //x^2 cooeficient
    number l; //x cooeficient

    friend std::ostream& operator<<(std::ostream& out, elliptic_curve curve) {
        out<< curve.k<< " "<< curve.l;
        return out;
    }
};

struct solution {
    number a, b, c;

    friend std::ostream& operator<<(std::ostream& out, solution s) {
        out<< "a="<<s.a<< ",b="<< s.b<< ",c="<< s.c;
        return out;
    }

    number value() {
        return a/(b + c) + b/(a + c) + c/(a + b);
    }
};

struct elliptic_point {
    elliptic_curve curve;
    number x, y;

    bool operator==(elliptic_point &other) {
        return x == other.x && y == other.y;
    }    
};

elliptic_curve make_curve(number n) {
    return elliptic_curve{4 * n * n + 12 * n - 3, 32 * (n + 3)};
}

elliptic_point transform(solution s, number n) {
    number x = -4 * (s.a + s.b + 2 * s.c) * (n + 3);
    x /= (2 * s.a + 2*s.b - s.c) + (s.a + s.b) * n;
    number y = 4 * (s.a - s.b) * (n + 3) * (2 * n + 5);
    y /= (2 * s.a + 2 * s.b - s.c) + (s.a + s.b) * n;
    elliptic_curve curve = make_curve(n);
    x.canonicalize();
    y.canonicalize();
    return elliptic_point{curve, x, y};
}

solution transform(elliptic_point p, number n) {
    number a = 8 * (n + 3) - p.x + p.y;
    a /= 2 * (4 - p.x) * (n + 3);
    number b = 8 * (n + 3) - p.x - p.y;
    b /= 2 * (4 - p.x) * (n + 3);
    number c = -4 * (n + 3) - (n + 2) * p.x;
    c /= (4 - p.x) * (n + 3);
    return solution{a, b, c};
}

elliptic_point double_point(elliptic_point p) {
    elliptic_curve c = p.curve;
    number lambda, new_x, new_y;
    lambda = 3 * p.x * p.x + 2 * c.k * p.x + c.l;
    lambda /= 2 * p.y;
    new_x = lambda * lambda - c.k - 2 * p.x;
    new_y = -lambda * new_x + lambda * p.x - p.y;
    return {c, new_x, new_y};
}


elliptic_point add(elliptic_point a, elliptic_point b) {
    if(a == b)
        return double_point(a);
    elliptic_curve c = a.curve;
    number lambda, new_x, new_y;
    lambda = b.y - a.y;
    lambda /= b.x - a.x;
    new_x = lambda * lambda - c.k - a.x - b.x;
    new_y = -lambda * new_x + lambda * a.x - a.y;
    return {c, new_x, new_y};
}

bool is_positive(solution s) {
    return sgn(s.a) == sgn(s.b) == sgn(s.c);
}

bool check_solution(solution s, number n) {
    return s.value() == n;
}


void print_sgn(solution s) {
    std::cout<< sgn(s.a) << " "<< sgn(s.b)<< " "<<sgn(s.c)<< "\n";
}


int main() {
    number n, a, b, c;
    std::cin>> n>> a>> b>> c;
    solution solution{a, b, c};
    elliptic_point point = transform(solution, n);
    elliptic_point step = point;

    while(!is_positive(solution)) {
        point = add(point, step);
        solution = transform(point, n);
    }
    number d = solution.a.get_den() * solution.b.get_den() * solution.c.get_den();
    
    std::cout<< solution.a * d<< "\n";
    std::cout<< solution.b * d<< "\n";
    std::cout<< solution.c * d<< "\n";
    return 0;
}