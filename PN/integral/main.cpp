#include <cassert>
#include <cstdio>
using namespace std;

double si(double);

int main()
{
    int z;
    for (assert(scanf("%d", &z) == 1); z; --z)
    {
        double x;
        assert(scanf("%lf", &x) == 1);
        printf("x = %.5lf, si(x) = %.15lf\n", x, si(x));
    }
    return 0;
}
