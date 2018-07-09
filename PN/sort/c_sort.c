#include<stdio.h>

void swap(long *a, long *b)
{
    long x = *a;
    *a = *b;
    *b = x;
}

int partition(long *data, unsigned long count)
{
    int mid = count >> 1;
    long x = data[mid];
    swap(data + mid, data + count - 1);
    int i = 0;
    int j = count - 2;

    while(i <= j)
    {
        while(i <= j && data[i] < x)
            i++;
        while(i <= j && data[j] >= x)
            j--;
        if(i > j)
            break;
        
        swap(data + i, data + j);
        i++;
        j--;
    }
        
    return i;
}

void i_sort(long *data, unsigned long count)
{
    for(int i = 1; i < count; i++)
    {
        int x = data[i];

        int j;
        for(j = i - 1; j >= 0 && data[j] > x; j--)
        {
            data[j+1] = data[j];
        }

        data[j+1] = x;
    }
            
}

void q_sort(long *data, unsigned long count)
{
    if(count <= 1)
        return;
    int s = partition(data, count);
    q_sort(data, s);
    q_sort(data + s + 1, count - s - 1);
}

void sort(long *data, unsigned long count)
{
    q_sort(data, count);
    i_sort(data, count);
}
