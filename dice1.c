
/* dice1.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int myRandom(int max)
{
    int x;
    x = rand() % max + 1;

    return x;
}

int main()
{
    int balance, bet, guess, dice;

    srand(getpid());

    balance = 1000;
    printf("Make your bet: ");
    fflush(stdout);
    scanf("%d", &bet);

    printf("make your guess 1 - 6, 0 to quit\n");
    scanf("%d", &guess);
    if (guess == 0)
      return 0;

    sleep(2);
    dice = myRandom(6);
    printf("%d\n", dice);

    if (guess != dice)
    {
        balance -= bet;
        printf("Sorry, you lost\n");
    }
    else
    {
        bet *= 3;
        balance += bet;
        printf("You won - congratulations!\n");
    }

    if (balance == 0)
    {
        printf("GAME OVER\n"); 
        return 0;
    }

    printf("Balance : $d\n", balance);
    return 0;
}
