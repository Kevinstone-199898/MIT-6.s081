## Code Analysis of File System

### c文件和头文件结构
* file.c
：    实现文件系统调用的支撑函数
    * fileinit
    * filealloc
    * filedup
    * fileclose
    * filestat
    * fileread
    * filewrite
* file.h
  * file ： 
  * inode ： 内存中的inode
* fs.c ： file system 的底层实现
    * 包含五层：
        * Blocks ：磁盘上的块
        * Log ：为了实现crash recovery的日志系统
        * Files ： 
        * Directories ： 文件夹（特殊的文件，其中的内容为当前文件夹下的文件/文件夹名）
        * Names(Path) ： 路径解析
    * 函数：
        * readsb ： 读超级块
        * fsinit ： 初始化文件系统
        * bzero : block清零
        * balloc ： 在硬盘中分配一个块

          <strong>对块的操作通常包含三个步骤：</strong>
            * bread
            * log_write
            * brelse
        * bfree ： 回收块
        * <strong>itable结构：内核中维护的一张inode的表，包含被所有进程使用的inode，还有ip->ref，ip->valid等信息</strong>
        * ialloc ：Returns an <strong>unlocked but allocated and referenced inode</strong>
        * iupdate ：Copy a modified in-memory inode to disk. <strong>Caller must hold ip->lock</strong>
        * iget ：找到在itable中的指定inode（如果已存在，ref+1；不存在则分配，ref=1）
        * idup ：ip->ref++
        * ilock ：Lock the given inode.<strong>Reads the inode from disk if necessary.</strong>
        * iunlock ：
        * iput ：ip->ref--，如果减到0了并且nlink为0，说明可以释放这个inode
        * iunlockput ：iunlock + iput
        * bmap ：返回inode中第n个块的地址
        * itrunc ：丢弃inode中的block
        * stati ：Copy stat information from inode.Caller must hold ip->lock.
        * readi ：Read data from inode.Caller must hold ip->lock.
        * writei ：Write data to inode.Caller must hold ip->lock.
        * dirlookup ：Look for a directory entry in a directory.
        * dirlink ：Write a new directory entry (name, inum) into the directory dp.
        * skipelem ：
        * namex ：
        * namei ：
        * nameiparent ： 

* fs.h
    * 硬盘的布局
：    [ boot block | super block | log | inode blocks | free bit map | data blocks]
    * dinode ： 硬盘中的inode
    * dirent ： 文件夹中包含一系列的dirent结构
* bio.c : Buffer Cache
    * binit ：
    * bget ：
    * bread ：
    * bwritre ：
    * brelse ：
    * bpin ：
    * bunpin ： 
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
* <strong>inode：能被使用需要经过五个步骤</strong>
    * 被分配：
    * 在itable中ref ： ip->ref tracks the number of in-memory pointers to the entry (open files and current directories). <strong>iget() finds or creates a table entry and increments its ref; iput() decrements ref.</strong>
    * valid ： <strong>ilock() reads the inode from the disk and sets ip->valid, while iput() clears ip->valid if ip->ref has fallen to zero.</strong>
 
一般的操作：
```c
ip = iget(dev, inum)
ilock(ip)
//   ... examine and modify ip->xxx ...
iunlock(ip)
iput(ip)
```
* file ：
* dirent

### 函数解析
* put()函数
```c
void iput(struct inode *ip)
{
  
}
```

### 重点问题说明

1. iunlockput和iunlock的区别
：    iunlockput比iunlock多了一个iput的操作，
3. inode的ref和nlink的区别
： ref指的是进程中用到的该文件的次数；nlink是指某文件在硬盘中有多少硬链接（有多少种不同的路径可以指向该文件）

