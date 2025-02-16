#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int countLetters(string text);
int countWords(string text);
int countSentences(string text);
int calculIndex(string text);


/*
 * This program calculates the readability grade level of a given text using the Coleman-Liau index:
 * index = 0.0588 * L - 0.296 * S - 15.8, where L is the average letters per 100 words and S is the
 * average sentences per 100 words. It prints "Grade X" (rounded) or "Grade 16+" for higher levels,
 * and "Before Grade 1" for lower levels. Words are space-separated sequences, and sentences end
 * with '.', '!', or '?'. Hyphenated words count as one. The program assumes no leading/trailing or
 * multiple consecutive spaces.
 */

int main(void)
{

    string textInput = get_string("Text: ");
    int indexRead = calculIndex(textInput); // Get index

    if (indexRead < 1) // If below 1 is before Grade 1, if above 16 it is grade 16+ and give the
                       // rounded grade otherwise
    {
        printf("Before Grade 1\n");
    }
    else if (indexRead > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", indexRead);
    }
}

// Count letters by counting alphabetical char
int countLetters(string text)
{
    int letters = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            letters += 1;
        }
    }
    return letters;
}

// Count words by counting spacing (+1)
int countWords(string text)
{
    int words = 1;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isspace(text[i]))
        {
            words += 1;
        }
    }
    return words;
}

// Count Sentences by counting ".","?","!"
int countSentences(string text)
{
    int sentences = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences += 1;
        }
    }
    return sentences;
}
// Calcul the index with the Coleman-Liau index and the number of letters/words/sentences in this
// text
int calculIndex(string text)
{
    float letters = countLetters(text);
    float words = countWords(text);
    float sentences = countSentences(text);
    float l = letters / words * 100;
    float s = sentences / words * 100;
    float index = 0.0588 * l - 0.296 * s - 15.8;
    return round(index);
}
