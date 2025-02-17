#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float averageValue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            averageValue =
                (float) (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            if (averageValue > 255)
            {
                averageValue = 255;
            }
            else if (averageValue < 0)
            {
                averageValue = 0;
            }
            int value = round(averageValue);

            image[i][j].rgbtBlue = value;
            image[i][j].rgbtGreen = value;
            image[i][j].rgbtRed = value;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int mirrorPixel = width - j - 1;
            if (mirrorPixel > width)
            {
                mirrorPixel = width;
            }
            else if (mirrorPixel < 0)
            {
                mirrorPixel = 0;
            }
            RGBTRIPLE placeholder;
            // Keep the color of the original pixel
            placeholder.rgbtBlue = image[i][j].rgbtBlue;
            placeholder.rgbtGreen = image[i][j].rgbtGreen;
            placeholder.rgbtRed = image[i][j].rgbtRed;
            // Put the colors of twin pixel to the original
            image[i][j].rgbtBlue = image[i][mirrorPixel].rgbtBlue;
            image[i][j].rgbtGreen = image[i][mirrorPixel].rgbtGreen;
            image[i][j].rgbtRed = image[i][mirrorPixel].rgbtRed;
            // Put the colors of the original piwel in the twin pixel
            image[i][mirrorPixel].rgbtBlue = placeholder.rgbtBlue;
            image[i][mirrorPixel].rgbtGreen = placeholder.rgbtGreen;
            image[i][mirrorPixel].rgbtRed = placeholder.rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE bufferimage[height][width];

    // Nested loop to iterate all pixels
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;

            float iteration = 0;
            // nested loop to check surrounding pixels
            for (int k = i - 1; k < i + 2; k++)
            {
                if (k >= 0 && k < height)
                {
                    for (int l = j - 1; l < j + 2; l++)
                    {
                        if (l >= 0 && l < width)
                        {
                            tempRed += image[k][l].rgbtRed;
                            tempBlue += image[k][l].rgbtBlue;
                            tempGreen += image[k][l].rgbtGreen;
                            iteration++;
                        }
                    }
                }
            }
            tempRed = (float) (tempRed / iteration);
            tempBlue = (float) (tempBlue / iteration);
            tempGreen = (float) (tempGreen / iteration);

            bufferimage[i][j].rgbtRed = round(tempRed);
            bufferimage[i][j].rgbtBlue = round(tempBlue);
            bufferimage[i][j].rgbtGreen = round(tempGreen);
        }
    }

    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = bufferimage[i][j].rgbtRed;
            image[i][j].rgbtBlue = bufferimage[i][j].rgbtBlue;
            image[i][j].rgbtGreen = bufferimage[i][j].rgbtGreen;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    float gxMatrix[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    float gyMatrix[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    // Nested loop to iterate all pixels
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;

            float tempGxRed = 0;
            float tempGxBlue = 0;
            float tempGxGreen = 0;
            float tempGyRed = 0;
            float tempGyBlue = 0;
            float tempGyGreen = 0;
            float iteration = 0;
            // nested loop to check surrounding pixels

            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {

                    int currentL = l - j + 1;
                    int currentK = k - i + 1;
                    if ((l >= 0 && l < width) && (k >= 0 && k < height))
                    {

                        tempGxRed += gxMatrix[currentK][currentL] * image[k][l].rgbtRed;
                        tempGyRed += gyMatrix[currentK][currentL] * image[k][l].rgbtRed;
                        tempGxBlue += gxMatrix[currentK][currentL] * image[k][l].rgbtBlue;
                        tempGyBlue += gyMatrix[currentK][currentL] * image[k][l].rgbtBlue;
                        tempGxGreen += gxMatrix[currentK][currentL] * image[k][l].rgbtGreen;
                        tempGyGreen += gyMatrix[currentK][currentL] * image[k][l].rgbtGreen;
                    }
                    else
                    {

                        tempGxRed += gxMatrix[currentK][currentL] * 0;
                        tempGyRed += gyMatrix[currentK][currentL] * 0;
                        tempGxBlue += gxMatrix[currentK][currentL] * 0;
                        tempGyBlue += gyMatrix[currentK][currentL] * 0;
                        tempGxGreen += gxMatrix[currentK][currentL] * 0;
                        tempGyGreen += gyMatrix[currentK][currentL] * 0;
                    }
                }
            }
            // Calcul the Gx and GY coef
            tempRed = (float) sqrt(pow(tempGxRed, 2) + pow(tempGyRed, 2));
            tempBlue = (float) sqrt(pow(tempGxBlue, 2) + pow(tempGyBlue, 2));
            tempGreen = (float) sqrt(pow(tempGxGreen, 2) + pow(tempGyGreen, 2));

            // Clamp each temp
            if (tempRed > 255)
            {
                tempRed = 255;
            }
            if (tempBlue > 255)
            {
                tempBlue = 255;
            }
            if (tempGreen > 255)
            {
                tempGreen = 255;
            }
            // Store it in a temp image to not change future surrounding pixel now
            tempImage[i][j].rgbtRed = round(tempRed);
            tempImage[i][j].rgbtBlue = round(tempBlue);
            tempImage[i][j].rgbtGreen = round(tempGreen);
        }
    }

    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tempImage[i][j].rgbtRed;
            image[i][j].rgbtBlue = tempImage[i][j].rgbtBlue;
            image[i][j].rgbtGreen = tempImage[i][j].rgbtGreen;
        }
    }

    return;
}
