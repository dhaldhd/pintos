#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

extern struct semaphore * ld_ex_sema;
#endif /* userprog/syscall.h */
