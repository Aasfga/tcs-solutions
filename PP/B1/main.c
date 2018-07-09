#include <stdio.h>
#include <stdlib.h>




void countedPrimes(char *sieve, int *dividers, int n)
{
     for(int i = 0; i < n; i++)
     {
         sieve[i] = 1;
     }
    sieve[0] = 0;
    sieve[1] = 0;
    dividers[1] = 1;

    for(int i = 2; i < n; i++)
    {
        if(sieve[i] == 1)
        {
            dividers[i] = i;
            for(int j = 2*i; j < n; j+=i)
            {
                if(sieve[j] == 1)
                    dividers[j] = i;

                sieve[j] = 0;
            }
        }
    }
}

void countPrimes(char *sieve, int *sum, int n)
{
    for(int i = 2; i < n; i++)
    {
        sum[i] = sum[i-1] + sieve[i];
    }
}

int main()
{
    const int N = 10000000;
    char array[N];
    int dividers[N];
    int suma[N];
    int sets;
    int a,b;

    countedPrimes(array, dividers, N);
    countPrimes(array, suma, N);
    scanf("%i", &sets);

    while(sets--)
    {
        scanf("%i %i", &a, &b);
        printf("%i %i %i \n", suma[b] - suma[a-1], dividers[a], dividers[b]);
    }


    return 0;
}
