#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int blockSize = 512;
int jpegCount = 0;
char *filename;

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage : ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("File can't be opened");
        return 2;
    }

    uint8_t buffer[blockSize];
    FILE *img;
    // While there's still data left to read from the memory card
    while (fread(buffer, 1, blockSize, card) == blockSize)
    {
        // New jpg is found
        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff) &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            // first jpg
            if (jpegCount == 0)
            {
                filename = malloc(8);
                sprintf(filename, "%03i.jpg", jpegCount);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Can't create images");
                    return 3;
                }
                fwrite(&buffer, blockSize, 1, img);
                jpegCount++;
            }
            else
            {
                fclose(img);

                sprintf(filename, "%03i.jpg", jpegCount);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Can't create images");
                    return 3;
                }
                fwrite(&buffer, blockSize, 1, img);
                jpegCount++;
            }
        }
        else if (jpegCount > 0)
        {
            fwrite(&buffer, blockSize, 1, img);
        }
    }
    fclose(img);
    fclose(card);

    free(filename);
}
