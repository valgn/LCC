#include <stdio.h>
int a = 0x7fffffff, b = 1;
int foo1()
{
    printf("Ejecutando foo1...\n");
    a = a + (b << 31);
    printf("0x %x\n", a);
    return a;
}
int foo2()
{
    printf("Ejecutando foo2...\n");
    a = a + b;
    printf("0x %x\n", a);
    return a;
}
int main()
{
    printf("0x %x 0x %x\n ", foo1(), foo2());
    return 0;
}