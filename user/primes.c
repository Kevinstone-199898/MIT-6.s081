#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p[2], num[50] = {0};
    for (int i = 2; i <= 35; i++)
        num[i - 2] = i;

    pipe(p);
    write(p[1], num, 34 * sizeof(int));
    close(p[1]);
    close(0);
    dup(p[0]);
    int pid;

    while (1)
    {
        if ((pid = fork()) == 0) // child process
        {
            // printf("pid: %d\n", getpid());
            int prime, x;
            if (!read(0, &prime, sizeof(int)))
            {
                // printf("pid: %d exiting!!!\n", getpid());
                exit(0);
            }

            printf("prime %d\n", prime);
            int pi[2];
            pipe(pi);

            while (read(0, &x, sizeof(int)))
            {
                if (x % prime)
                {
                    write(pi[1], &x, sizeof(int));
                    // printf("writing %d\n", x);
                }
            }
            close(0);
            dup(pi[0]);
            close(pi[0]);
            close(pi[1]);
        }
        else
        {
            wait(0);
            // printf("pid: %d waiting for pid %d!!!\n", getpid(), pid);
            exit(0);
        }
    }

    return 0;
}