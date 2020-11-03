#include <stdio.h>

int testing(int a, int b)
{
    printf("%i\n", a + b);
    return a;
}

int main(int argc, char **argv)
{
    testing(3, 9);
}