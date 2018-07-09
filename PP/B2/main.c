#include <stdio.h>


void setZero(char array[], int a, int b)
{
    for(int i = a; i <= b; i++)
    {
        array[i] = 0;
    }
}

int main()
{
    int a,b;
    int d;
    int dividers;
    int set;

    scanf("%i", &set);
    while(set--)
    {
        scanf("%i %i %i", &a, &b, &dividers);
        char array[b - a + 1];
        setZero(array, 0, b - a);
        int endArray = b - a;
        for(int i = 0; i < dividers; i++)
        {
            scanf("%i", &d);

            int startJ = (a - 1)/d;
            startJ *= d;
            startJ += startJ == 0 ? 0 : d;
            startJ -= a;


            for(int j = startJ; j <= endArray; j+= d)
            {
                array[j] = 1;
            }
        }

        for(int i = 0; i <= endArray; i++ )
        {
            if(array[i] == 1)
            {
                printf("%i ", i + a);
            }
        }

        printf("\n");
    }


    return 0;
}