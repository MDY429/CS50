#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    string plaintext;
    
    if(argc != 2)
    {
        printf("%s k\n",argv[0]);
        return 1;
    }
    
    int k = atoi(argv[1]);
    
    printf("plaintext: ");
    plaintext = get_string();
    
    printf("ciphertext: ");
    for(int i = 0, n = strlen(plaintext); i< n; i++)
    {
        if(isalpha(plaintext[i]) && isupper(plaintext[i]))
        {
            printf("%c", ((plaintext[i] - 65 + k) % 26) +65);
        }
        else if((plaintext[i]) && islower(plaintext[i]))
        {
            printf("%c",((plaintext[i] - 97 + k) % 26) +97);
        }
        else
        {
            printf("%c",plaintext[i]);
        }
    }
    printf("\n");
    
    return 0;
}