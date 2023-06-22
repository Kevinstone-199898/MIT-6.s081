## Code Analysis of File System

### 结构

### 函数解析

```c
if (omode & O_CREATE)
{
    ip = create(path, T_FILE, 0, 0);
    if (ip == 0)
    {
      end_op();
      return -1;
    }
}
```

### 重点问题说明
