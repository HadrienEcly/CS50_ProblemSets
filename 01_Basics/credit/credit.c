#include <cs50.h>
#include <math.h>
#include <stdio.h>

// Prototype
int GetLength(long number);
int AddDigit(long number);
int CalculDoubled(long number);
int CalculRest(long number);
bool CheckLuhnLegit(long cardnumber);
int GetFirstTwoDigit(long cardnumber);
void CheckCard(long cardnumber);

int main(void)
{
    int LuhnNumber;
    long cardnumber;
    do // Input card number
    {
        cardnumber = get_long("Number: ");
    }
    while (cardnumber < 0);

    if (CheckLuhnLegit(cardnumber)) // If the checksum is correct (I named it Luhn Number) check which card it is
    {
        CheckCard(cardnumber);
    }
    else
    {
        printf("INVALID\n");
    }
}

int GetLength(long number) // Get Length of a number by dividing per 10
{
    int length = 0;
    do
    {
        number = number / 10;
        length++;
    }
    while (number > 0);
    return length;
}

int CalculDoubled(long number) // Calcul first half of luhn's number (Double and add every other digits)
{
    int length = GetLength(number);
    int doubledResult = 0;
    for (int i = 1; i < length; i = i + 2)
    {
        long digit;
        long power = pow(10, i);
        digit = number / power;
        digit = digit % 10;
        doubledResult = doubledResult + AddDigit(digit);
    }
    return doubledResult;
}
int CalculRest(long number) // Calcul second half of luhn's number (add the rest)
{
    int length = GetLength(number);
    int restResult = 0;
    for (int i = 0; i < length; i = i + 2)
    {
        long digit;
        long power = pow(10, i);
        digit = number / power;
        digit = digit % 10;
        restResult = restResult + digit;
    }
    return restResult;
}

int AddDigit(long number) //Add all digits of a number between 0 and 99
{
    int newNumber;
    int length;
    newNumber = number * 2;
    length = GetLength(newNumber);
    if (length > 1)
    {
        long firstDigit;
        long secondDigit;
        long finalNumber;
        firstDigit = newNumber % 10;
        secondDigit = newNumber / 10 % 10;
        finalNumber = firstDigit + secondDigit;
        return finalNumber;
    }
    else
    {
        return newNumber;
    }
}
bool CheckLuhnLegit(long cardnumber) // Cgheck if the checksum (Luhn's number) is correct and makes it maybe a legit credit card
{
    int LuhnNumber;
    LuhnNumber = CalculDoubled(cardnumber) + CalculRest(cardnumber);
    if (LuhnNumber % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int GetFirstTwoDigit(long cardnumber) // Get the first two digit to check which card it is later
{
    int length;
    long firstTwoDigit;
    length = GetLength(cardnumber);
    firstTwoDigit = cardnumber / pow(10, length - 2);
    firstTwoDigit = firstTwoDigit % 100;
    return firstTwoDigit;
}

void CheckCard(long cardnumber)
{
    int StartNumber = GetFirstTwoDigit(cardnumber);
    int length = GetLength(cardnumber);

    if ((StartNumber == 34 || StartNumber == 37) && length == 15) // Amex check
    {
        printf("AMEX\n");
    }
    else if ((StartNumber > 50 && StartNumber < 56) && length == 16) // Mastercard check
    {
        printf("MASTERCARD\n");
    }
    else if ((StartNumber > 39 && StartNumber < 50) && (length == 13 || length == 16)) // Visa check
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
