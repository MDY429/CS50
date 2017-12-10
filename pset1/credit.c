#include <cs50.h>
#include <stdio.h>

int CheckCard(long long number);
bool Verify(long long number);

int main(void)
{
    long long creditnum;
    int card;
    
    do
    {
        printf("Number: ");
        creditnum = get_long_long();
    }
    while(creditnum < 0);
    
    if(Verify(creditnum))
    {
        card = CheckCard(creditnum);
        switch(card)
        {
            case 4:
                printf("VISA\n");
                break;
            case 34:
            case 37:
                printf("AMEX\n");
                break;
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                printf("MASTERCARD\n");
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

int CheckCard(long long number)
{
    int quotient;
    
    quotient = (number / 1000000000000);
    
    for(int i = 0; i < 3; i ++)
    {
        if(quotient < 100 && (quotient <39 || quotient > 50)) //0~39,50~99 for MASTERCARD, AMEX and VISA
        {
            break;
        }
        quotient /= 10;
    }
    return quotient;
}

bool Verify(long long number)
{
    long long digits = 0;
    long long count = 1;
    int evensumdigits = 0;
    int sum = 0;
    
    //Check even digits.
    for(int i = 0; i < 8; i++)
    {
        digits = (number % (100 * count) - number % (10 * count)) / (10 * count);
        count *= 100;
        evensumdigits = (digits * 2) / 10 + (digits * 2) % 10;
        sum += evensumdigits;
    }
    
    count = 1;
    
    //Check odd digits.
    for(int i = 0; i < 8; i++)
    {
        digits = (number % (10 * count) - number % (1 * count)) / (1 * count);
        count *= 100;
        sum += (int)digits;
    }
    
    if((sum % 10) == 0)//Check last digit whether is 0.
    {
        return true;
    }
    else
    {
        return false;
    }
}