/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    
    float resize = atof(argv[1]);
    if(resize <= 0 || resize > 100)
    {
        fprintf(stderr, "Usage: must be a floating-point value in [0.0, 100.0],\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (int)(bi.biWidth * resize * sizeof(RGBTRIPLE)) % 4) % 4;
    int headersize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    int32_t new_biWidth = bi.biWidth * resize;
    int32_t new_biHeight = bi.biHeight * resize;
    float shrink = 1 / resize;

    if(resize >= 1)// for expanding
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            for(int m = 0 ; m < resize; m ++)
            {
                //To load the resize times org row.
                fseek(inptr, headersize + (i * (bi.biWidth * sizeof(RGBTRIPLE) + padding)), SEEK_SET);
                
                // iterate over pixels in scanline
                for (int j = 0; j < bi.biWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                    // write RGB triple to outfile for resize times.
                    for(int n = 0; n < resize; n++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
    
                // then add it back (to demonstrate how)
                for (int k = 0; k < new_padding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
        }
    }
    else//for shrinking
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight * resize; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0, count = bi.biWidth; j < bi.biWidth * resize; j++, count -= shrink)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
                if(count > 0)
                {
                    //move the position.
                    fseek(inptr, (shrink - 1) * 3, SEEK_CUR);
                }
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
            // skip for shrinking size.
            fseek(inptr, (bi.biWidth * sizeof(RGBTRIPLE) + padding), SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }
    
    //Overrdie the header information.
    fseek(outptr, 0, SEEK_SET);
    
    // override outfile's BITMAPFILEHEADER
    bf.bfSize = headersize + ((new_biWidth * sizeof(RGBTRIPLE)) + new_padding) * abs(new_biHeight);
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // override outfile's BITMAPINFOHEADER
    bi.biSizeImage = ((new_biWidth * sizeof(RGBTRIPLE)) + new_padding) * abs(bi.biHeight) * resize;
    bi.biWidth = new_biWidth;
    bi.biHeight = new_biHeight;
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
