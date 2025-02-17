
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
    WAV Volume Adjustment Program

    This program modifies the volume of a WAV file by a given factor.
    WAV files store audio as a sequence of 16-bit samples, and adjusting
    the volume is done by multiplying each sample by the given factor.

    Steps:
    1. Read the 44-byte WAV file header and write it unchanged to the output file.
    2. Read each 16-bit audio sample from the input file.
    3. Multiply each sample by the given factor to adjust the volume.
    4. Write the modified samples to the output file.

    Assumptions:
    - The WAV file format follows a standard 16-bit sample structure.
    - Memory management should be handled properly to prevent leaks.

    Example Usage:
    ./volume input.wav output.wav 2.0  // Doubles the volume
    ./volume input.wav output.wav 0.5  // Reduces the volume by half
*/


// Number of bytes in .wav header
const int HEADER_SIZE = 44;
// The header is 44 byte (44 8bit)
uint8_t header[HEADER_SIZE];
int16_t buffer;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // Update volume of sample
        buffer *= factor;
        // Write updated sample to new file
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }
    // Close files
    fclose(input);
    fclose(output);
}
