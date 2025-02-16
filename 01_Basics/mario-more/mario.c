#include <stdio.h>
#include <cs50.h>

//Placeholders
void printNone(int times);
void printBrick(int times);
void printPyramide(int height);

int main(void)
{
    int answer;
    do
    {
        answer = get_int("Height: ");
    }
    while (answer < 1 || answer > 8 );
    printPyramide(answer);

}

//Functions
void printNone(int times)
{
    for(int i = 0; i < times; i++)
    {
        printf(" ");
    }
}
void printBrick(int times)
{
    for(int i = 0; i < times; i++)
    {
        printf("#");
    }
}

void printPyramide(int height)
{
    for(int i = 1; i < height+1; i++) // height +1 because otherwise it would count only 7 row instead of 8 as i start at 1
    {
        const int gap = 2;
        printNone(height-i);
        printBrick(i);
        printNone(gap);
        printBrick(i);
        printf("\n");
    }
}
