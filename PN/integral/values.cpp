#include<iostream>



int main()
{
    for(int i = 0; i < 16; i++)
    {
        double x = (double) 1/(2*i + 1);
        double f = 1;

        for(int j = 1; j <= 2*i+1; j++)
            f *= j;
        x /= f;
        unsigned long long *new_x = reinterpret_cast<unsigned long long*>(&x);
        std::cout<< std::hex<<"\tdq 0x" <<*new_x<< ",\n";
    }
    return 0;
}
