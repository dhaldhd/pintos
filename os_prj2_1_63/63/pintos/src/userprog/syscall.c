#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#define PF_P 0x1    /* 0: not-present page. 1: access rights violation. */
#define PF_W 0x2    /* 0: read, 1: write. */
#define PF_U 0x4    /* 0: kernel, 1: user process. */

//page_fault;

typedef int pid_t;

static void syscall_handler (struct intr_frame *);
int read(int, void * , unsigned );
void halt(void);
static int write (int fd, const void *buffer, unsigned size);
void myexit(int status);
pid_t exec(const char *cmd_line);
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}
	

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
//	unsigned int * uesp = f->esp;
//		printf("werwerwer\n");
	int * iesp = f->esp;
// bool  iswrite = (f->error_code & PF_W) != 0;
//bool  isuser = (f->error_code & PF_U) != 0;
//page_fault
// printf ("system call! %x\n",iesp);
 // printf("wer %d, %u\n",*iesp,*uesp);
//	hex_dump(iesp,iesp,10,1);
//	if(iesp>=PHYS_BASE || iesp<0) myexit(-1);
//	else
if(iesp>=PHYS_BASE || iesp < 0) {/*printf("111\n");*/ myexit(-1);}
	//		printf("iswrite ? %d isuser ? %d\n",iswrite,isuser);
//if(iswrite && isuser)
//{
//}
//else if(!iswrite && isuser)
//{
//		myexit(-1);
//}
//page_fault;
  switch(*iesp)
  {
		case SYS_WAIT :
			//printf("wait dump\n");
//			hex_dump(iesp, iesp ,256,1);
			f->eax = wait(*(iesp+1));
			break;
		case SYS_HALT : 

			//printf("halt\n");
			halt();
			break;
		case SYS_EXEC :
	//		printf("exec dump\n");
//			hex_dump(iesp, iesp, 256, 1);

//			if(iesp+1>PHYS_BASE) myexit(-1);
//			printf("\n\n\n\n\n\n\n\nexec %s\n", *(iesp+1));
//			page_fault
			f->eax = exec(*(iesp+1));
		  break;
		case SYS_EXIT :
		  	//printf("exit!!!!!\n");
//			if(iesp+1>PHYS_BASE) myexit(-1);
//		  	hex_dump(iesp, iesp, 256,1);
			myexit(*(iesp+1));
	//	  	printf("%s: exit(%d)\n",thread_name(), *(iesp+1));
			//thread_exit();
		  break;
		case SYS_WRITE :
		  	//printf("write\n");
		//	hex_dump(iesp,iesp,256,1);
			f->eax = write(*(iesp+5),*(iesp+6),*(iesp+7));
//			f->eax = sys_write(f);
		  break;
		case SYS_READ :
		  //hex_dump(iesp,iesp,256,1);
			//printf("read\n");
		  f->eax = read(*(iesp+5), *(iesp+6), *(iesp+7));
		  //	printf("read\n");
		  break;
		case SYS_FIBO :
	//	  printf("fibo!\n");
	//	  f->eax = 1;
	//	 printf("야\n");
		 // hex_dump(iesp, iesp, 256, 1);
		  f->eax = fibo(*(iesp+1));
		  break;
		case SYS_SUM : 
		  //printf("sum!\n");
	//	  f->eax = 1;
		  //printf("호\n");
		//  hex_dump(iesp, iesp, 256, 1);
		  f->eax = sum(*(iesp+6),*(iesp+7),*(iesp +8), *(iesp+9));
		  break;
		default : //printf("no handler for this syscall\n");
		  break;
  }  
//	thread_exit ();
  //printf("system call end\n");
}
int fibo(int n)
{
		if(n == 0) return 0;
		else if( n==1) return 1;
		else return (fibo(n-1) + fibo(n-2));
}
int sum(int a, int b, int c, int d)
{
		return a+b+c+d;
}
int wait(pid_t pid)
{
//		printf("설마 웨잍 시발\n");
		int tmp = process_wait(pid);
//printf("설마 웨잍\n");
		//printf("sys call wait %d\n", thread_current()->tid);
		return tmp;
}
pid_t exec(const char *cmd_line)
{
		//printf("exec cmd %s\n", cmd_line);
		return	process_execute(cmd_line);
}
void myexit(int status)
{
		//printf("2222%d\n", status);
	if(status < 0 ) status = -1;

	printf("%s: exit(%d)\n",thread_name(), status);
//	process_exit();
		thread_current()->parent->dyingstatus = status;
	thread_exit();
}
void halt(void)
{
		shutdown_power_off();
}
int read(int fd, void *buffer, unsigned size)
{
		if(fd == 0)
		input_getc();
		return size;
}
static int write (int fd, const void *buffer, unsigned size)
{
		char* buf =buffer;
		//printf("ddd %d\n", fd);		
		if(fd==1)		putbuf(buffer,size);

		return size;
}
