#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"
#include "memlayout.h"

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_mmap(void)
{
  // addr = 0,the kernel should decide the virtual address at which to map the file
  // len: num of bytes to map(might not be same as size of file)
  // prot: writable, readable……
  // flags:
  //      shared(1)—modifications to the mapped memory should be written back to the file
  //      private(2)
  // fd: open file descriptor
  // off = 0

  uint64 addr;
  int len, prot, flags, fd, off;
  if (argaddr(0, &addr) < 0 || argint(1, &len) < 0 || argint(2, &prot) < 0 ||
      argint(3, &flags) < 0 || argint(4, &fd) < 0 || argint(5, &off) < 0)
    return ((uint64)-1);
  struct proc *proc = myproc();
  struct file *file = proc->ofile[fd];

  // check the prot
  if (((!file->readable) && (prot & 1)) || ((!file->writable) && (prot & 2) && (flags & 1)))
  {
    return ((uint64)-1);
  }

  int np = (len + PGSIZE - 1) / PGSIZE;
  //   find unused user virtual address for the file
  uint64 p = proc->sz;
  /*for (int i = 0; i < 16; i++)
  {
    struct vma *v = &proc->vma[i];
    uint64 st = v->addr, ed = st + v->len;
    if (addr >= st && addr + len <= ed && v->len)
    {
      printf("already mapped\n");
      return v->addr;
    }
  }*/
  struct vma vma;
  vma.len = len;
  vma.addr = p;
  vma.perm = prot;
  vma.flag = flags;
  vma.file = file;
  vma.off = 0;
  for (int i = 0; i < 16; i++)
  {
    if (proc->vma[i].len == 0)
    {
      proc->vma[i] = vma;
      break;
    }
  }
  filedup(vma.file);
  proc->sz += PGSIZE * np;
  return vma.addr;
}

uint64
sys_munmap(void)
{
  uint64 addr;
  int len;
  if (argaddr(0, &addr) < 0 || argint(1, &len) < 0)
    return -1;

  return sysmunmap(addr, len);
}
