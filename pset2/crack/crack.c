#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _XOPEN_SOURCE

char *crypt(const char *key, const char *salt);


int main(int argc, string argv[])
{   
    
    char salt[2],
         password[5],
         key[] = {
             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0'
         };
         
    if(argc != 2)
    {
        printf("hash\n");
        return 1;
    }
    else
    {
        strncpy(salt, argv[1], 2);

        //find the all of possible password.
        for(int i = 0; i < 53; i++)
        {
            password[0] = key[i];
            for(int j = 0; j < 53; j++)
            {
                password[1] = key[j];
                for(int k = 0; k < 53; k++)
                {
                    password[2] = key[k];
                    for(int l = 0; l < 53; l++)
                    {
                        password[3] = key[l];
                        //check whether same with input hash.
                        if(strcmp(argv[1], crypt(password, salt)) == 0)
                        {
                            printf("%s\n",password);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    
    printf("Cannot crack hash\n");
    return 1;
    
    
}