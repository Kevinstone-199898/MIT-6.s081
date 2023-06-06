#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p1[2], p2[2];
    char buf1 = 'I', buf2;

    pipe(p1);
    pipe(p2);
    close(0);
    dup(p1[0]);
    write(p1[1], &buf1, 1);
    close(p1[0]);
    close(p1[1]);

    if (fork() == 0) // child process
    {
        read(0, &buf2, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &buf2, 1);
        exit(0);
    }
    else // parent process
    {
        wait(0);
        read(p2[0], &buf1, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}