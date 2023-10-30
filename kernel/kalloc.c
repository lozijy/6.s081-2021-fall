// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

//用于释放一个物理内存页面
//首先，通过一系列检查确保传入的物理地址 pa 是有效的。它会检查物理地址是否按页对齐（((uint64)pa % PGSIZE) != 0），是否位于内核结束位置 end 之后（(char*)pa < end），以及是否超出了物理内存的限制 PHYSTOP（(uint64)pa >= PHYSTOP）。如果这些检查失败，将触发 panic，表示出现了错误。

//在释放内存之前，使用 memset() 函数将页面中的内容填充为 1。这样做是为了捕捉可能存在的悬空引用，即指针仍然指向已释放的内存。

//将物理页面的结构 struct run 转换为指针 r，这是因为物理页面的结构 struct run 用于在物理页面之间维护一个链表，以便在需要时将其加入到可用内存空闲列表中。

//获取内核内存分配器的锁 kmem.lock，以确保在修改可用内存列表时不会发生竞争条件。

//将当前释放的页面 r 插入到可用内存的空闲列表 kmem.freelist 的开头。
//输入的指针是物理
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
//用于分配一个 4096 字节大小的物理内存页面
//获取内核内存分配器的锁 kmem.lock，以确保在修改可用内存列表时不会发生竞争条件。

//从可用内存的空闲列表 kmem.freelist 中获取一个空闲页面的指针 r。

//如果成功获取到一个空闲页面 r，则将空闲列表的头指针 kmem.freelist 移动到下一个空闲页面（即 r->next）。

//释放内核内存分配器的锁，允许其他线程继续对可用内存列表进行修改。

//如果成功获取到一个空闲页面 r，则使用 memset() 函数将页面中的内容填充为 5。这样做是为了将页面初始化为一个特定的值（即 "junk"），以便在使用页面之前进行清理。

//最后，将页面的指针 r 作为函数的返回值返回给调用者。
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
