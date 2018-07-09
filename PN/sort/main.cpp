#include <cstdio>
using namespace std;

extern "C" void sort(long * data, unsigned long count);

int main()
{
    unsigned long count;
    scanf("%lu", &count);
    long data[count];
    for (unsigned long i = 0; i < count; ++i)
        scanf("%ld", &data[i]);
    sort(data, count);
    for (unsigned long i = 0; i < count; ++i)
        printf("%ld ", data[i]);
    printf("\n");
    return 0;
}
