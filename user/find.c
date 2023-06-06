#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path) // 得到文件名或者文件夹名
{
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    return ++p;
}

void find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if (!strcmp(fmtname(path), target))
            printf("%s\n", path);
        return;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) // 读目录下的文件（文件夹）
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (stat(buf, &st) < 0) // 返回iNode的引用数量
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (!strcmp(p, ".") || !strcmp(p, "..") || st.type == 3)
                continue;

            if (st.type == 2)
            {
                if (!strcmp(p, target))
                    printf("%s\n", buf);
                continue;
            }
            find(buf, target);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "error when executing find: no enough input!\n");
        exit(-1);
    }

    char *path = argv[1];
    char *target = argv[2];
    find(path, target);

    exit(0);
}