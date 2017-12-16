#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: argument should be 2.\n");
        return 1;
    }
    
    char *infile = argv[1];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // Buffer for checking header.
    uint8_t buffer_h[4] = {0};
    // Buffer for checking block.
    uint8_t buffer_b[512] = {0};
    
    while(1)
    {
        fread(buffer_h, 4, 1, inptr);
        
        // Check JPEG header.
        if(buffer_h[0] == 0xff && buffer_h[1] == 0xd8 && buffer_h[2] == 0xff && (buffer_h[3] & 0xf0) == 0xe0)
        {
            // If yes, roll back 4 bytes.
            fseek(inptr, -4, SEEK_CUR);
            break;
        }
    }
    
    for(int i = 0, block = 512; block == 512 ; i++)
    {
        char *outfile = malloc(sizeof(char));
        
        // Name the output file.
        sprintf(outfile,"%03d.jpg", i);
        
        // open output file
        FILE *outptr = fopen(outfile, "w");
        if (outptr == NULL)
        {
            fclose(inptr);
            fprintf(stderr, "Could not create %s.\n", outfile);
            return 3;
        }
        
        for(int j = 0 ;; j++)
        {
            block = fread(buffer_b, 1, block, inptr);
            
            // If block less than 512, then EOF.
            if(block < 512)
            {
                break;
            }
            
            // Checking JPEG header.
            if(buffer_b[0] == 0xff && buffer_b[1] == 0xd8 && buffer_b[2] == 0xff && (buffer_b[3] & 0xf0) == 0xe0)
            {
                if(j > 0)
                {
                    // Roll back the 512 bytes except first round.
                    fseek(inptr, -512, SEEK_CUR);
                    break;
                }
            }
            
            fwrite(buffer_b, sizeof(buffer_b), 1, outptr);
        }
        
        // free the allocate memory.
        free(outfile);
        // close outfile
        fclose(outptr);
    }
    
    // close infile
    fclose(inptr);
    
    // success
    return 0;
}