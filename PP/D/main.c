#include <stdio.h>


double binSearch(int volume, double prec)
{
    double left = 0;
    double right = volume + 1;
    double s;
    double sVol;

    while(right - left >= prec)
    {
        s = (right + left)/2;
        sVol = s*s*s - s*s + s;

        if(sVol < volume)
        {
            left = s;
        }
        else //if(sVol> volume)
        {
            right = s;
        }
    }

    return right;
}

int main()
{
    double prec = 0.0000000001;
    int sets;
    double result;
    int floor;
    int ceiling;
    int volume;

    scanf("%i", &sets);

    while(sets--)
    {
        scanf("%i", &volume);
        result = binSearch(volume, prec);
        floor = (int) result;
        double floorVol = floor*floor*floor - floor*floor + floor;


        if(floorVol == volume)
            ceiling = floor;
        else if(floorVol > volume)
        {
            floor--;
            ceiling = floor + 1;
        }
        else
            ceiling = floor + 1;


        /*

        if(floorVol == volume)
            ceiling = floor;
        else
            ceiling = floor + 1;
         */
        printf("%.10f %i %i\n", result, floor, ceiling);
    }



    return 0;
}