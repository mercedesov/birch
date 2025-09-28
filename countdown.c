
/* countdown.c */
#include <stdio.h>
#include <unistd.h>

int main()
{
    int number;

    number = 5;
    while (number > 0)
    {
        printf("%d\n", number);
        sleep(1);
        number = number - 1;
    }

    puts("Blastoff");

    return 0;
}
