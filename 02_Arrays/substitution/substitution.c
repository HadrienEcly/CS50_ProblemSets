#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * This program encrypts messages using a substitution cipher, replacing each letter based on a
 * 26-character key provided as a command-line argument. The key must contain each letter exactly
 * once and be case-insensitive. The program prompts for plaintext input, encrypts it while
 * preserving case and non-alphabet characters, and outputs the ciphertext. Errors occur for
 * invalid keys or incorrect arguments.
 */


string cipherTheText(string txt, string key);
bool isAllNotAlpha(string str);
bool isidenticalchar(string str);

int main(int argc, string argv[])
{
    // Enter correct argument
    if (argc != 2)
    {
        printf("Usage ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (isAllNotAlpha(argv[1]))
    {
        printf("Key must only contain alphabetic letters.\n");
        return 1;
    }
    else if (isidenticalchar(argv[1]))
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    string key = argv[1];
    string plaintxt = get_string("plaintext: ");
    printf("ciphertext: %s\n", cipherTheText(plaintxt, key));
}

//Encrypt the text given
string cipherTheText(string txt, string key)
{
    string cipherTxt = txt;
    for (int i = 0, len = strlen(txt); i < len; i++)
    {
        if (isalpha(txt[i]))
        {
            if (islower(txt[i]))
            {
                cipherTxt[i] = tolower(
                    key[txt[i] - 97]); // Le char dans cipher devient la version lower de la même
                                       // position de la lettre dans l'alphabet que la key
            }
            if (isupper(txt[i]))
            {
                cipherTxt[i] = toupper(key[txt[i] - 65]);
            }
        }
        else
        {
            cipherTxt[i] = txt[i];
        }
    }
    return cipherTxt;
}

//check for identical char in a string
bool isidenticalchar(string str)
{

    for (int i = 0, len = strlen(str); i < len; i++)
    {

        for (int l = 0; l < len; l++)
        {
            if (i != l && str[i] == str[l])
            {
                return true;
            }
        }
    }
    return false;
}

//check if all not alpha
bool isAllNotAlpha(string str)
{

    for (int i = 0, len = strlen(str); i < len; i++)
    {
        if (!isalpha(str[i]))
        {
            return true;
        }
    }
    return false;
}
