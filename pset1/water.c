#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int minute = 0;
    do{
    printf("Minutes: ");
        minute = get_int();
    }while(minute <= 0);
    printf("Bottles: %i\n", minute * 12);
}

/*
int n;
    do
{
    n = get_int();
}
    while(n < 0 );
    printf("minutes : %i\n", n );
    printf("bottles : %i\n", n*12 );
*/