#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p[2];
    char buf[512];

    pipe(p);
    int pid = fork();
    if (pid == 0) // child process
    {
        int child_pid = getpid();
        if (read(p[0], buf, 1) == 0) // read byte from pipe
        {
            printf("error!\n");
            exit(-1);
        }
        else
        {
            fprintf(2, "%d: received ping\n", child_pid);
            write(p[1], buf, 1); // write byte on the pipe
            exit(0);
        }
    }
    else if (pid > 0) // parent process
    {
        int parent_pid = getpid();
        write(p[1], "hello world", 1); // send byte
        wait(0);
        read(p[0], buf, 1); // read byte from pipe
        fprintf(2, "%d: received pong\n", parent_pid);
        exit(0);
    }
    else
    {
        printf("error!\n");
        exit(-1);
    }

    return 0;
}