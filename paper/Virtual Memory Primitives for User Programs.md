
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
Programming languages with garbage collector: go, python, java……, but not c(malloc andd free) and rust.

* Divide the heap into <strong>to-space </strong> and <strong> from-space</strong>
* Baker's algorithm —— real-time, incremental GC
* using vm primitives to reduce cost:
    * divide the to-space into <strong>scanned and unscanned</strong>
    * Trick of concurrency: <strong>unscannded area is not mapped</strong> —— map2 comes in

### Shared virtual memory
* maintain single-write and multiple-reader coherence in multicore or a network of computers
* a page currently being written can reside in only one processor's memory
* the shared memory only exits virtually
  
### Concurrent checkpoint
