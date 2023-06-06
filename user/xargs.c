#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAXARGS 10

int main(int argc, char *argv[])
{
    char buf[512], cur;
    char *eargv[MAXARGS];
    int p = 0;

    memcpy(eargv, &argv[1], (argc - 1) * sizeof(char *));

    while (read(0, &cur, sizeof(char)))
    {
        if (cur == '\n')
        {
            eargv[argc - 1] = buf;
            if (fork() == 0)
            {
                exec(argv[1], eargv);
            }
            else
            {
                wait(0);
                eargv[argc - 1] = 0;
                p = 0;
                memset(buf, 0, sizeof(buf));
            }
        }
        else
            buf[p++] = cur;
    }
    exit(0);
}