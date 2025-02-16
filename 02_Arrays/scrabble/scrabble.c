#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>


/*
 * This program simulates a simple Scrabble game where two players input words, and their scores
 * are calculated based on letter values (A=1, B=3, ..., Z=10). The player with the higher score
 * wins, with ties handled accordingly. The program prompts for two words, computes scores, and
 * prints "Player 1 wins!", "Player 2 wins!", or "Tie!" based on the results.
 */

// Prototypes
int CalculResult(string word);

int points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");
    int result1 = CalculResult(word1);
    int result2 = CalculResult(word2);
    if (result1 > result2)
    {
        printf("Player 1 wins!\n");
    }
    else if (result2 > result1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

//Calcul points for each char of a word
int CalculResult(string word)
{
    int score = 0;
    int asciiChar = 0;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        word[i] = toupper(word[i]);

        if (isalpha(word[i]))
        {
            asciiChar = word[i] - 65;
            score += points[asciiChar];
        }
    }
    return score;
}
