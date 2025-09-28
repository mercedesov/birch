
#include <stdio.h>

#define F fflush(stdout)
#define ADDITION 1
#define SUBTRACTION 2
#define DIVISION 3
#define MULTIPLICATION 4 

void addition(int *target, int a, int b)
{
    *target = a + b;
}

void subtraction(int *target, int a, int b)
{
    *target = a - b;
}

void multiplication(int *target, int a, int b)
{
    *target = a * b;
}

void division(int *target, int a, int b)
{
    if (b != 0)
        *target = a / b;
    else
        *target = 0;  // Or handle error
}

int main()
{
    int x, y, op, result;
    void (*fp)(int*, int, int);

    printf("Choose your opt\n"); F;
    scanf("%d", &op);

    switch (op)
    {
        case ADDITION:
            fp = addition;
            break;
        case SUBTRACTION:
            fp = subtraction;
            break;
        case DIVISION:
            fp = division;
            break;
        case MULTIPLICATION:
            fp = multiplication;
            break;
        default:
            return 0;
    }

    printf("Number 1: "); F;
    scanf("%d", &x);
    printf("Number 2: "); F;
    scanf("%d", &y);

    fp(&result, x, y);
    printf("Result: %d\n", result);

    return 0;
}
