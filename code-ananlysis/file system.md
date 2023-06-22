## Code Analysis of File System

### c文件和头文件结构
* file.c
* file.h
* fs.c
：    file system 的实现函数，包含五层：
    * Blocks ：磁盘上的块
    * Log ：为了实现crash recovery的日志系统
    * Files ： 
    * Directories ： 文件夹（特殊的文件，其中的内容为当前文件夹下的文件/文件夹名）
    * Names(Path) ： 路径解析
* fs.h
* sysfile.c
:    包含跟文件相关的系统调用函数和其他支撑函数
  
系统调用函数：
  * sys_dup
  * sys_read
  * sys_write
  * sys_close
  * sys_fstat
  * sys_link
  * sys_unlink
  * sys_open
  * sys_mkdir
  * sys_mknod
  * sys_chdir
  * sys_exec
  * sys_pipe
  * sys_symlink(fs lab)

其它函数：
  * argfd
  * fdalloc
  * isdirempty
  * create
  

### 结构解析
* inode
* dinode
* file
* 
* itable:
* dirent

### 函数解析
* put()函数
```c
void iput(struct inode *ip)
{
  acquire(&itable.lock);

  if (ip->ref == 1 && ip->valid && ip->nlink == 0)
  {
    // inode has no links and no other references: truncate and free.

    // ip->ref == 1 means no other process can have ip locked,
    // so this acquiresleep() won't block (or deadlock).
    acquiresleep(&ip->lock);

    release(&itable.lock);

    itrunc(ip);
    ip->type = 0;
    iupdate(ip);
    ip->valid = 0;

    releasesleep(&ip->lock);

    acquire(&itable.lock);
  }

  ip->ref--;
  release(&itable.lock);
}
```

### 重点问题说明

1. iunlockput 和 iunlock 的区别：
iunlockput比iunlock多了一个iput的操作，
