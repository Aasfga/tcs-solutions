#include <string.h>
#include <stdio.h>

void toLower(char array[])
{
      int i = 0;
      while(array[i] != '\0')
      {
              array[i] = array[i] < 97 ? array[i] + 32 : array[i];
              i++;
      }
}

void insertionSortS(char array[][31], int size)
{
      for(int i = 1; i < size; i++)
      {
              char value[31];
              strcpy(value, array[i]);

              int j;
              for(j = i; j > 0 && strcmp(value, array[j - 1]) < 0; j--)
              {
                      strcpy(array[j], array[j - 1]);
              }

              strcpy(array[j], value);
      }
}

int countStrings(char strings[][31], int size, char countedStrings[][31], int count[])
{
      if(size == 0)
              return 0;
      int countSize = 0;

      strcpy(countedStrings[0], strings[0]);
      count[0] = 1;

      for(int i = 1; i < size; i++)
      {
              if(strcmp(countedStrings[countSize], strings[i]) == 0)
              {
                      count[countSize]++;
              }
              else
              {
                      countSize++;
                      count[countSize] = 1;
                      strcpy(countedStrings[countSize], strings[i]);
              }
      }

      return ++countSize;
}

void insertionSortI(int count[], int position[], int size)
{
      for(int i = 1; i < size; i++)
      {
              int value = position[i];
              int j;
              for(j = i; j > 0 && count[value] > count[position[j - 1]];)
              {
                      position[j] = position[j - 1];
                      j--;
              }

              position[j] = value;
      }
}

int main()
{
      int sets;
      scanf("%i", &sets);

      while(sets--)
      {
              int arraySize;
              scanf("%i", &arraySize);
              char array[arraySize][31];
              char countedStrings[arraySize][31];
              int count[arraySize];

              for(int i = 0; i < arraySize; i++)
              {
                      scanf("%s", array[i]);
                      toLower(array[i]);
              }

              insertionSortS(array, arraySize);
              int size = countStrings(array, arraySize, countedStrings, count);
              int position[size];

              for(int i = 0; i < size; i++)
              {
                      position[i] = i;
              }
              insertionSortI(count, position, size);

              printf("%i\n", size);
              for(int i = 0; i < size; i++)
              {
                      printf("%s %i\n", countedStrings[position[i]], count[position[i]]);
              }
      }

      return 0;
}
