//#include "userprog/syscall.h"
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
#define getStruct(SAVEPTR,THISLIST, ID, STRUCT, STRUCT_ELEM, STRUCT_ID)			\
{	\
		struct list_elem * e;	\
		STRUCT * target;	\
		for( e = list_begin(&(THISLIST)); e != list_end (&(THISLIST)); e = list_next(e) )	\
		{	\
				target = list_entry(e, STRUCT,STRUCT_ELEM); 	\
				if(target->STRUCT_ID == ID)	\
						break;	\
		}	\
		if(e == list_end(&(THISLIST)))	\
		{	\
			target = NULL; \
		} \
		SAVEPTR = target; \
}



struct semaphore * ld_ex_sema = NULL;
unsigned tell(int fd);
static void syscall_handler (struct intr_frame *);
int read(int, void * , unsigned );
void halt(void);
static int write (int fd, const void *buffer, unsigned size);
void myexit(int status);
pid_t exec(const char *cmd_line);
bool remove(const char *file);

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
		case SYS_SEEK :
			seek(*(iesp+4), *(iesp+5));
			break;
		case SYS_TELL : 
			f->eax =tell(*(iesp+1));
			break;
		case SYS_FILESIZE :
			f->eax = filesize(*(iesp+1));
			break;
		case SYS_REMOVE :
			f->eax = remove(*(iesp+1));
			break;
		case SYS_CLOSE : 
			close(*(iesp+1));
			break;
		case SYS_OPEN :
			f->eax = open(*(iesp+1));
			break;
		case SYS_CREATE : 
			//hex_dump(iesp,iesp, 256,1);
			if(*(iesp+4)){	
					f->eax = filesys_create(*(iesp+4), *(iesp+5));
			}//else f->eax = false;
			else
			{
				//	printf("!!!!!!%s %d\n", *(iesp+4), *(iesp+5));
					myexit(-1);
					f->eax = false; 
			}
			break;
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
		   // printf("!!!!wr\n");
			f->eax = write(*(iesp+5),*(iesp+6),*(iesp+7));
//			f->eax = sys_write(f);
		  break;
		case SYS_READ :
		 // hex_dump(iesp,iesp,256,1);
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
void seek(int fd, unsigned position)
{
		struct myfile *mf;
		getStruct(mf, thread_current()->filelist, fd, struct myfile, fe, fd);
		file_seek(mf->fp,position);
}
unsigned tell(int fd)
{
		struct myfile *mf;
		getStruct(mf, thread_current()->filelist, fd, struct myfile, fe, fd);
		return file_tell(mf->fp);
}
int filesize(int fd)
{
		int size;
		struct myfile * mf;
		getStruct(mf, thread_current()->filelist, fd, struct myfile, fe, fd);
		size = file_length(mf->fp);

//		printf("따끈따끈 size %d\n", size);
		return size;
}
bool remove(const char *file)
{
		return filesys_remove(file);
}
void close(int fd)
{
		struct list_elem * e;
		struct thread * currTh = thread_current();
		struct myfile * tfile;	
		for( e = list_begin(&(currTh->filelist)); e != list_end (&(currTh->filelist)); e = list_next(e) )
		{
				tfile = list_entry(e, struct myfile, fe); 
				if(tfile->fd == fd)
						break;
		}

		if(e == list_end(&(currTh->filelist)))
		{
			//	tchild->exit_status = -1;
				//printf("길을 잃었따 \n");

		
		}
		else
		{
				list_remove(e);
//				printf("!!!!!!!\n");
				file_close(tfile->fp);
		}


}
/*struct void * getStruct(struct list thislist , int id,void * aux, void * aux_elem, void * aux_id)
{
		struct list_elem * e;
		struct void * target;
//		struct thread * currTh = thread_current();
		struct myfile * tfile;	
		for( e = list_begin(&thislist); e != list_end (&thislist); e = list_next(e) )
		{
				target = list_entry(e, aux,aux_elem); 
				if(target->aux_id == id)
						break;
		}

		if(e == list_end(&thislist))
		{
			//	tchild->exit_status = -1;
				//printf("길을 잃었따 \n");
			return NULL;
		
		}
		else
		{
				return target;
		}


}*/
int  open(const char *file)
{
		struct file * f;
		struct myfile * mf;
//		printf("start");
		if(!file) 
		{
				myexit(-1);
				return -1;
		}
		sema_down(ld_ex_sema);
		f = filesys_open(file);
		sema_up(ld_ex_sema);
		//printf("open.");
		if(f)
		{
			//printf("f");
			mf = (struct myfile * )malloc(sizeof(struct myfile));
			if(mf)
			{
					
				mf->fd = (thread_current()->currfd)++;
				mf->fp = f;
			//	printf("연다 파일 %d\n", mf->fd);
				list_push_back(&(thread_current()->filelist),&(mf->fe));
				//printf("mf\n");
			}
			else return -1;
		}
		else
		{
				return -1;
		}
	//	printf("!!!%d\n", mf->fd);
		return mf->fd;
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
	//page_fault;
	//	printf("exec cmd %s\n", cmd_line);
		return	process_execute(cmd_line);
}
void myexit(int status)
{
		struct thread * cur = thread_current();
	//	printf("2222%d\n", status);
/*  if(!(cur->parent->iswaiting))
  {
		  cur->iszombi = true;
		  printf("좀비되라고 미친노마\n");
		  intr_disable();
		  sema_down(&(cur->zombisema));

		  printf("움직인다~~~~\n");
  }
 */
	if(status < 0 ) status = -1;

//	printf("살아있네%d\n", status);
	if(!(cur->is_par_waiting))
  	{
		  cur->iszombi = true;
	//	  printf("좀비되라고 미친노마%d %d\n", status,thread_current()->tid);
		  intr_disable();
		  sema_down(&(cur->zombisema));

		 // cur->parent->iswaiting = false;
	//	  printf("움직인다~~~~\n");
  }
 
	printf("%s: exit(%d)\n",thread_name(), status);
	thread_current()->parent->dyingstatus = status;
//	process_exit();
	thread_exit();
}
void halt(void)
{
		shutdown_power_off();
}
int read(int fd, void *buffer, unsigned size)
{
		struct myfile * mf;
		if(buffer >= PHYS_BASE) 
		{
				myexit(-1);
				return 0;
		}
		if(fd == 0)
			input_getc();
		else
		{
			getStruct(mf,thread_current()->filelist, fd, struct myfile, fe, fd);	
			/*{
					struct list_elem *e;
					struct myfile * target;
					for( e = list_begin(&(thread_current()->filelist)); e != list_end(&(thread_current()->filelist)); e = list_next(e))
					{
							target = list_entry(e, struct myfile, fe);
							if(target->fd == fd)
									break;
					}

					if(e==list_end(&(thread_current()->filelist)))
					{
							target = NULL;
					}
					mf = target;
			}*/
			if(mf)
			{
					file_read(mf->fp, buffer, size);
			}

		}
//		printf("size!!! %d\n", size);
		return size;
}
static int write (int fd, const void *buffer, unsigned size)
{
		char* buf =buffer;
		struct myfile * mf;
	//	printf("ddd %d\n", fd);		
		if(fd==1)		putbuf(buffer,size);
		else
		{
				getStruct(mf, thread_current()->filelist, fd, struct myfile, fe, fd);
				if(mf)
				{
					size = file_write(mf->fp, buffer, size);
				}
		}
		
		return size;
	//	return 0;
}
