/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

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
    int swapcount = -1, temp = 0;
    
    for(int i = 1; i <= n; i++)
    {
        swapcount = 0;
        
        for(int j = 1; j <= n-i; j++ )
        {
            if(values[j-1] > values[j])
            {
                //SWAP
                temp = values[j-1];
                values[j-1] = values[j];
                values[j] = temp;
                
                swapcount++;
            }
        }

        //if SWAP == 0 , done for sorting, break.
        if(swapcount == 0)
        {
            break;
        }
    }
    
    return;
}
