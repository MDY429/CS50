#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _XOPEN_SOURCE

char *crypt(const char *key, const char *salt);

int main(void)
{
    char salt[2] = {'5','0'};
    string s = get_string();
    string r;

    r = crypt(s,salt);
    
    printf("%s\n",r);
    printf("%s\n",s);
          
}
