
## Abstract
What vm primitives should OS provide to user programs?

## VM Primitives
* trap
* prot1
* protn
* unprot
* dirty
* map2 : map the same physical page at two different virtual addresses at different levels of protection

<strong> Will not violate the isolation of processes and kernel space because the user program still only have access to its own address space even with these vm primitives</strong>

## VM applications

### Concurrent garbage collection

### Shared virtual memory

### Concurrent checkpoint
