#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What's your name ? ");
    printf("Hello, %s",name);
    printf("\n"); //New line here, couldn't put it on line 7 cause check50 didn't liked it

}
