#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int change = 0;
    int coin = 0;
    int remaind = 0;
    
    do
    {
        printf("O hai! How much change is owed?\n");
        change = get_float() *100 + 0.5;
    }
    while(change < 0);
        
    //Quarters can be used.
    remaind = change / 25;
    coin += remaind;
    change %= 25;
    
    //dimes can be used.
    remaind = change / 10;
    coin += remaind;
    change %= 10;
    
    //nickels can be used.
    remaind = change / 5;
    coin += remaind;
    change %= 5;
    
    //pennies can be used.
    remaind = change / 1;
    coin += remaind;
    change %= 1;
    
    //Print the coin you have.
    printf("%i\n",coin);
}