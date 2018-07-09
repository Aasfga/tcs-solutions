#import <stdio.h>


int binSearchLesser(int *array, int n, int x) {
    int l = -1;
    int p = n - 1;
    int s;

    while (l != p) {
        s = (l + p + 1) / 2;

        if (array[s] <= x)
            l = s;
        else
            p = s - 1;
    }

    return l;
}

int binSearchGreater(int *array, int n, const int x)
{
    int l = 0;
    int p = n;
    int s;

    while(l != p)
    {
        s = (l + p - 1)/2;

        if(array[s] >= x)
            p = s;
        else
            l = s + 1;
    }

    return l;
}

int main()
{
    int sets;
    int trees;
    int asksNumber;
    int a;
    int b;


    scanf("%i", &sets);


    while(sets--)
    {
        int distance = 0;
        scanf("%i", &trees);
        int array[trees];

        for(int i = 0; i < trees; i++)
        {
            scanf("%i", &array[i]);
            array[i] += distance;
            distance = array[i];
        }

        scanf("%i", &asksNumber);

        for(int i = 0; i < asksNumber; i++)
        {
            scanf("%i %i", &a, &b);
            int result = binSearchLesser(array, trees, b) - binSearchGreater(array, trees, a);
            result++;
            printf("%i\n", result);
        }
    }


    return 0;
}
