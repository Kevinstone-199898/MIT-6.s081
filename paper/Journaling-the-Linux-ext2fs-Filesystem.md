## 论文简介

[ Journaling the Linux ext2fs Filesystem](https://pdos.csail.mit.edu/6.828/2021/readings/journal-design.pdf)

ex2fs文件系统的问题：
* non-atomic
* unpredictable

## 分析

### 现存问题（目的）
avoid the need for an expensive recovery stage every time a machine reboots

### 优化文件系统的几个原则
* Performance
* Compatability
* Reliability
  * Peservation
  * Predicitability : requires that the filesystem must make its writes to disk in a predictable order whenever a single update operation changes multiple blocks on disk
      * <strong>synchronous</strong> ：wait for the first writes to complete before submitting the next ones to the device driver。

        问题：性能差, no batch up
      * <strong> deferred ordered write</strong>: maintain an ordering between the disk buffers in memory, and to ensure that when we do eventually go to write back the data, we never write a block until all of its predecessors are safely on disk
 
        问题：cyclic dependencies between cached buffers
      * <strong> soft update</strong>:

      以上三种方法存在的问题： the recovery process still has to scan the entire disk in order to find and repair any uncompleted operations
  * Atomicity
 
### Log-structured FS——Batch, Commit
* Anatomy of transactions
    * ordering(dependency) is still important:  one transaction deleting a filename from one block in a directory and another transaction inserting the same filename into a different block
* 文件系统中的transaction和数据库中的transaction的不同
    * No transaction abort——文件系统中transaction前会保证合法性；DB中可能存在半途中的abortion
    * short life term of transaction in FS : 定期的，batch的transaction（多久一次transaction——tradeoff）


