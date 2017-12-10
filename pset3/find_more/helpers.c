/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

#define MAX_NUM 65535
/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int start, end, middle;
    
    start = 0;
    end = n - 1;
    
    while(start <= end)
    {
        middle = (start + end) / 2;
        
        //if value > middle of arry, let start = middle + 1
        if(value > values[middle])
        {
            start = middle + 1;
        }
        //if value < middle of arry, let end = middle - 1
        else if(value < values[middle])
        {
            end = middle - 1;
        }
        //if value = middle of arry, return true.
        else
        {
            return true;
        }
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int count[MAX_NUM + 1] = {0};
    int counter = 0;
    
    //put the value to counting array.
    for(int i = 0; i < n; i++)
    {
        count[values[i]] ++;
    }
    
    //Sorting
    for(int i = 0; i <= MAX_NUM; i++)
    {
        for(int j = count[i]; j > 0; j--, counter++)
        {
            //To let array to be sorted.
            values[counter] = i;
        }
    }
    
    return;
}
