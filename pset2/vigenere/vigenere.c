#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int CheckCommand(int cmd, string cmdstr[]);
char Enchiper(char p, string enc, int j, int k);

int main(int argc, string argv[])
{
    int chkcmd;
    string plaintext;
    
    chkcmd = CheckCommand(argc, argv);
    if(chkcmd == 1)
    {
        return 1;
    }
   
    printf("plaintext: ");
    plaintext = get_string();
    
    printf("ciphertext: ");
    for(int i = 0, j = 0, n = strlen(plaintext), k = strlen(argv[1]); i < n; i++)
    {
        if(isalpha(plaintext[i]))
        {
            plaintext[i] = Enchiper(plaintext[i], argv[1], j, k);
            j++;
        }
        
        printf("%c",plaintext[i]);
    }
    printf("\n");
    
    return 0;
}

int CheckCommand(int cmd, string cmdstr[])
{
    //Ony pass with two element.
    if(cmd != 2)
    {
        printf("%s k\n",cmdstr[0]);
        return 1;
    }
    
    for(int i = 0, n = strlen(cmdstr[1]); i < n; i++)
    {
        //return 1 if there have any integer.
        if(isalpha(cmdstr[1][i]) == false)
        {
            printf("%s k\n",cmdstr[0]);
            return 1;
        }
    }
    return 0;
}

char Enchiper(char p, string enc, int j, int k)
{
    char s;
    if(isupper(p))
    {
        //((input - 'A')+(encode - 'A'))% 26 -> check shift, then +65 -> ciphertext
        s = ((p - 65 + (toupper(enc[j % k]) - 65)) % 26) +65;
    }
    if(islower(p))
    {
        s = ((p - 97 + (toupper(enc[j % k]) - 65)) % 26) +97;
    }
    return s;
}