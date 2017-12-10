#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    
    if(s[0] != ' ')
    {
        printf("%c", toupper(s[0]));
    }
    
    for(int i = 0, n = strlen(s); i < n; i++)
    {
        if(s[i] == ' ')
        {
            if(s[i+1] != ' ' && (i+1) < n)
            {
                printf("%c",toupper(s[i+1]));
            }
        }
    }
    printf("\n");
}