// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// 208 is 26*8 because hash function will calculate the sum of the 8 first letters

const unsigned int FACTOR = 8;
const unsigned int N = 26 * FACTOR;

int number_of_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index;
    index = hash(word);
    node *cursor;
    cursor = table[index];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int index = 0;
    int maxIteration = 0;
    // Base case
    if (strlen(word) >= FACTOR)
    {
        maxIteration = FACTOR;
    }
    else
    {
        maxIteration = strlen(word);
    }
    for (int i = 0; i < maxIteration; i++)
    {
        index += toupper(word[i]) - 'A';
    }

    if (index > N - 1)
    {
        index = N - 1;
    }
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Error : Can't open the dictionnary");
        return false;
    }

    node *buffer = malloc(sizeof(node));
    if (buffer == NULL)
    {
        printf("Memory error");
        return false;
    }

    // Read each word in the file
    while (fscanf(source, "%s", buffer->word) != EOF)
    {
        int index;
        node *new_word;
        // Allocate memory for the word
        new_word = malloc(sizeof(node));
        if (new_word == NULL)
        {
            free(buffer);
            printf("Memory error");
            return false;
        }

        // Fill new node
        strcpy(new_word->word, buffer->word);
        new_word->next = NULL;

        // Add each word to the hash table
        number_of_words++;
        index = hash(new_word->word);
        new_word->next = table[index];
        table[index] = new_word;
    }
    free(buffer);
    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return number_of_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor;
        node *tmp;

        if (table[i] == NULL)
        {
            continue;
        }

        cursor = table[i];
        tmp = cursor;
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
