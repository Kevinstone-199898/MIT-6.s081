## 论文简介

[ Journaling the Linux ext2fs Filesystem](https://pdos.csail.mit.edu/6.828/2021/readings/journal-design.pdf)

## 分析

### 现存问题（目的）
avoid the need for an expensive recovery stage every time a machine reboots

### 优化文件系统的几个原则
* Performance
* Compatability
* Reliability
  * Peservation
  * Predicitability
  * Atomicity 
