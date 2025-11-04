#include <stdio.h>
#include <stdlib.h>
int calculo(char a, char b, char c, int d, char e, float f, double g, int h,
            int i, int j)
{
    return (a + c + d) * (i + j);
}

int main()
{
    int e = 5;
    return calculo(1, 2, 3, 4, 5, 1.0, 2.0, 6, 7, 8);
}
