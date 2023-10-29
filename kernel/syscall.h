//这个文件编写了几个宏，用于指明系统调用号
//执行ecall指令跳转到内核空间前需要将系统调用号保存到a7中(usys.S把这些汇编指令映射为函数，然后在user.h中声明了这些汇编函数供用户空间使用) li a7, SYS_trace
//然后会被保存在一个特殊的页面(trapframe陷阱框架)中，这个页面在内核空间和物理空间具有相同的映射，
//内核代码通过函数artint、artaddr和artfd从陷阱框架中检索第n个系统调用参数并以整数、指针或文件描述符的形式保存。他们都调用argraw来检索相应的保存的用户寄存器（kernel/syscall.c:35）。
// System call numbers
#define SYS_fork    1
#define SYS_exit    2
#define SYS_wait    3
#define SYS_pipe    4
#define SYS_read    5
#define SYS_kill    6
#define SYS_exec    7
#define SYS_fstat   8
#define SYS_chdir   9
#define SYS_dup    10
#define SYS_getpid 11
#define SYS_sbrk   12
#define SYS_sleep  13
#define SYS_uptime 14
#define SYS_open   15
#define SYS_write  16
#define SYS_mknod  17
#define SYS_unlink 18
#define SYS_link   19
#define SYS_mkdir  20
#define SYS_close  21
#define SYS_trace 22
#define SYS_sysinfo 24