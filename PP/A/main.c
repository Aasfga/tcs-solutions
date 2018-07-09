//
// Created by Dominik Grybos on 05.10.2016.
//

#include <stdio.h>

int nwd(int a, int b)
{
}

 

int main()
{
    int set;
    int a;
    char con;

    scanf("%i", &set);

    while(set--)
    {
        scanf("%i", &a);
        if(a == 1)
            printf("1 jest zlozona\n");
        else if(a % 2 == 0)
        {
            if(a == 2)
            {
                printf("2 jest pierwsza\n");
            }
            else
            {
                printf("%i jest zlozona\n", a);
            }
        }
        else
        {
            con = 0;
            for(int j = 3; j*j <= a; j += 2)
            {
                if(a % j == 0)
                {
                    con = 1;
                    break;
                }

            }
            if(con == 0)
            {
                printf("%i jest pierwsza\n", a);
            }
            else
            {
                printf("%i jest zlozona\n", a);
            }
        }
    }



    return 0;
}
