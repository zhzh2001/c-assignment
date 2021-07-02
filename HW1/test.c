#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(NULL));
    printf("%d\n", 0x7fff);
    for (int i = 0; i < 100; i++)
        printf("%d ", rand());
    puts("");
    printf("▌");
    return 0;
}