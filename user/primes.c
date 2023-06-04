#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    /*int p[2];
    int buf[512];
    int num[512];
    pipe(p);

    for (int i = 2; i <= 35; i++)
        num[i - 2] = i;

    write(p[1], num, 34 * sizeof(int));
    read(p[0], buf, 34 * sizeof(int));
    for (int i = 0; i <= 35; i++)
        fprintf(2, "%d ", buf[i]);
    fprintf(2, "\n");*/

    int buf[20];

    int p[2];
    pipe(p);

    int *point;
    int x = 2;

    for (point = &x; x <= 10; x++)
    {
        printf("writing %d\n", *point);
        write(p[1], point, sizeof(int));
    }
    read(p[0], buf, 9 * sizeof(int));

    for (int i = 0; i < 9; i++)
        printf("%d ", buf[i]);
    printf("\n");

    return 0;
}