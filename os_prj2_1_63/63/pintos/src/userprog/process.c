#include "userprog/process.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/synch.h"

//page_fault;

static thread_func start_process NO_RETURN;
static bool load (const char *cmdline, void (**eip) (void), void **esp);

/* Starts a new thread running a user program loaded from
   FILENAME.  The new thread may be scheduled (and may even exit)
   before process_execute() returns.  Returns the new process's
   thread id, or TID_ERROR if the thread cannot be created. */
tid_t
process_execute (const char *file_name) 
{
  char *fn_copy, * my_fn;
  tid_t tid;
  int i;
	//printf("exec %x %s\n", thread_current()->tid, file_name);
  my_fn = (char*)malloc(sizeof(char)*strlen(file_name));
  /* Make a copy of FILE_NAME.
     Otherwise there's a race between the caller and load(). */
  fn_copy = palloc_get_page (0);
  if (fn_copy == NULL)
    return TID_ERROR;
  strlcpy (fn_copy, file_name, PGSIZE);
  //printf("%s\n",file_name);
  for(i = 0;i <strlen(file_name)+1; i++)
  {
		  if(file_name[i] == ' ')
		  {
				  my_fn[i] = '\0';
				  break;
		  }
		  else
		  {
				  my_fn[i] = file_name[i];

				 // printf("!!!!!!!! %c\n", my_fn[i]);
		  }
  }
//  printf("!!!!%s\n", my_fn);
  /* Create a new thread to execute FILE_NAME. */
  //printf("\n\n\n\n\n\n\n\n\nfilename ::: !!!! %s\n", my_fn);
  if(!filesys_open(my_fn)) tid = TID_ERROR;
  else tid = thread_create (my_fn, PRI_DEFAULT, start_process, fn_copy);

  //printf("\n\n\n\n\n\n\n\n\nthththt ::: !!!! %s\n", my_fn);
  if (tid == TID_ERROR)
    palloc_free_page (fn_copy); 
  
  //printf("\n\n\n\n\n\nwerwerewrrwwrwrerwer     %d\n", tid);
  return tid;
}

/* A thread function that loads a user process and starts it
   running. */
static void
start_process (void *file_name_)
{
  char *file_name = file_name_;
  struct intr_frame if_;
  bool success;

  //printf("start\n\n\n\n\n");
  /* Initialize interrupt frame and load executable. */
  memset (&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;
  success = load (file_name, &if_.eip, &if_.esp);

  /* If load failed, quit. */
  palloc_free_page (file_name);
  if (!success) 
    thread_exit ();

  /* Start the user process by simulating a return from an
     interrupt, implemented by intr_exit (in
     threads/intr-stubs.S).  Because intr_exit takes all of its
     arguments on the stack in the form of a `struct intr_frame',
     we just point the stack pointer (%esp) to our stack frame
     and jump to it. */
  asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
  NOT_REACHED ();
}

/* Waits for thread TID to die and returns its exit status.  If
   it was terminated by the kernel (i.e. killed due to an
   exception), returns -1.  If TID is invalid or if it was not a
   child of the calling process, or if process_wait() has already
   been successfully called for the given TID, returns -1
   immediately, without waiting.

   This function will be implemented in problem 2-2.  For now, it
   does nothing. */
int
process_wait (tid_t child_tid UNUSED) 
{
/*	long long int i = 100000000;
 while(i-- > 0){
 }  
  return -1;*/
	//find this sema
		struct thread *tchild;
		struct list_elem *e;
		struct thread * currTh = thread_current();
		//printf("\n\n wait!!!!!!!!!!!!!!!!!!   %x\n", child_tid);
		for( e = list_begin(&(currTh->children)); e != list_end (&(currTh->children)); e = list_next(e) )
		{
				tchild = list_entry(e, struct thread, sibling_elem); 
				if(tchild->tid == child_tid)
						break;
		}

		if(e == list_end(&(currTh->children)))
		{
			//	tchild->exit_status = -1;
				//printf("길을 잃었따 \n");
				return -1;
		}


		//printf("im waiting and im %x my child %x.\n", thread_current()->tid, child_tid);
		sema_down(&(thread_current()->smph));
		//printf("세마세마 끈났다능 %x\n", thread_current()->tid);
		//printf("세마세마세마 끈났다능\n");
//		printf("printf!!!!!!! %x %x\n", tchild->status, currTh->dyingstatus);
		return currTh->dyingstatus; 
	//	return tchild->status;
	// return -1;
	//	sema_up();

}

/* Free the current process's resources. */
void
process_exit (void)
{
  struct thread *cur = thread_current ();
  uint32_t *pd;
  int i;

  /**나 생각좀**/
  /****/
  /* Destroy the current process's page directory and switch back
     to the kernel-only page directory. */
  pd = cur->pagedir;
  if (pd != NULL) 
    {
      /* Correct ordering here is crucial.  We must set
         cur->pagedir to NULL before switching page directories,
         so that a timer interrupt can't switch back to the
         process page directory.  We must activate the base page
         directory before destroying the process's page
         directory, or our active page directory will be one
         that's been freed (and cleared). */
      cur->pagedir = NULL;
      pagedir_activate (NULL);
      pagedir_destroy (pd);
    }
  ///////yj
//printf("im %x and my par %x.\n", cur->tid, cur->parent->tid);
//  sema_up(&(cur->parent->smph));

  intr_disable();
for(i = 0;i<(int)cur/(int)cur;i++)
{
  list_remove(&cur->sibling_elem);
 if(cur) sema_up(&(cur->parent->smph));
  thread_block();
  //////////
}
}

/* Sets up the CPU for running user code in the current
   thread.
   This function is called on every context switch. */
void
process_activate (void)
{
  struct thread *t = thread_current ();

  /* Activate thread's page tables. */
  pagedir_activate (t->pagedir);

  /* Set thread's kernel stack for use in processing
     interrupts. */
  tss_update ();
}

/* We load ELF binaries.  The following definitions are taken
   from the ELF specification, [ELF1], more-or-less verbatim.  */

/* ELF types.  See [ELF1] 1-2. */
typedef uint32_t Elf32_Word, Elf32_Addr, Elf32_Off;
typedef uint16_t Elf32_Half;

/* For use with ELF types in printf(). */
#define PE32Wx PRIx32   /* Print Elf32_Word in hexadecimal. */
#define PE32Ax PRIx32   /* Print Elf32_Addr in hexadecimal. */
#define PE32Ox PRIx32   /* Print Elf32_Off in hexadecimal. */
#define PE32Hx PRIx16   /* Print Elf32_Half in hexadecimal. */

/* Executable header.  See [ELF1] 1-4 to 1-8.
   This appears at the very beginning of an ELF binary. */
struct Elf32_Ehdr
  {
    unsigned char e_ident[16];
    Elf32_Half    e_type;
    Elf32_Half    e_machine;
    Elf32_Word    e_version;
    Elf32_Addr    e_entry;
    Elf32_Off     e_phoff;
    Elf32_Off     e_shoff;
    Elf32_Word    e_flags;
    Elf32_Half    e_ehsize;
    Elf32_Half    e_phentsize;
    Elf32_Half    e_phnum;
    Elf32_Half    e_shentsize;
    Elf32_Half    e_shnum;
    Elf32_Half    e_shstrndx;
  };

/* Program header.  See [ELF1] 2-2 to 2-4.
   There are e_phnum of these, starting at file offset e_phoff
   (see [ELF1] 1-6). */
struct Elf32_Phdr
  {
    Elf32_Word p_type;
    Elf32_Off  p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
  };

/* Values for p_type.  See [ELF1] 2-3. */
#define PT_NULL    0            /* Ignore. */
#define PT_LOAD    1            /* Loadable segment. */
#define PT_DYNAMIC 2            /* Dynamic linking info. */
#define PT_INTERP  3            /* Name of dynamic loader. */
#define PT_NOTE    4            /* Auxiliary info. */
#define PT_SHLIB   5            /* Reserved. */
#define PT_PHDR    6            /* Program header table. */
#define PT_STACK   0x6474e551   /* Stack segment. */

/* Flags for p_flags.  See [ELF3] 2-3 and 2-4. */
#define PF_X 1          /* Executable. */
#define PF_W 2          /* Writable. */
#define PF_R 4          /* Readable. */
void myStrcpy(char* dest, char* src);
static bool setup_stack (void **esp);
static bool validate_segment (const struct Elf32_Phdr *, struct file *);
static bool load_segment (struct file *file, off_t ofs, uint8_t *upage,
                          uint32_t read_bytes, uint32_t zero_bytes,
                          bool writable);


void myStrcpy(char* dest, char* src)
{
		int i = 0;
		for(i = 0;src[i]!='\0';i++)
		{
				dest[i] = src[i];
		}
		dest[i] = '\0';
}

/* Loads an ELF executable from FILE_NAME into the current thread.
   Stores the executable's entry point into *EIP
   and its initial stack pointer into *ESP.
   Returns true if successful, false otherwise. */
bool
load (const char *file_name, void (**eip) (void), void **esp) 
{
  struct thread *t = thread_current ();
  struct Elf32_Ehdr ehdr;
  struct file *file = NULL;
  off_t file_ofset;
  bool success = false;
  int i, j, argc = 1;
  char ** argv;
  char myfn[100];

  /* Allocate and activate page directory. */

  //printf("lalalala\n\n\n\n");
  t->pagedir = pagedir_create ();
  if (t->pagedir == NULL) 
    goto done;
  process_activate ();
  /*parse filename*/
  ///////////////////yj

  //printf("weifwoefjfowejfojweofjef여기다1 \n");
/*  for(j = 0;j<100;j++)
  {
		  if(file_name[j] == ' ' || file_name[j] == '\0')
		  {
				 myfn[j] = '\0';
				 break;
		  }
		  else
		  {
				  myfn[j] = file_name[j];
		  }
  }
*/
 {
		 char *token, *save_ptr, *ptr;
		 char * fn, **tempToks;
		 int i; 
		 argc = 0;
		 fn = (char*)malloc(sizeof(char)*strlen(file_name)+1);
		 tempToks = (char**)malloc(sizeof(char*)*strlen(file_name));
		 strlcpy(fn, file_name, strlen(file_name)+1);
		 fn[strlen(file_name)] = '\0';
		 //printf("%s\n", fn);
		 
		 //printf("여기다 2 \n");
		 save_ptr = NULL;

		 for(token = strtok_r(fn, " ", &save_ptr); token !=NULL;
						 token = strtok_r(NULL, " \n", &save_ptr))
		 {
				 //printf("토크나이즈 들어왔다 %s. %s.\n", token,*(& save_ptr));
				 tempToks[argc] = (char*)malloc(sizeof(char)*strlen(token));
				 strlcpy(tempToks[argc], token, strlen(token)+1);
				 argc++;
				 //printf("!!!!%s\n", tempToks[argc-1]);
		 }
		
		 //printf("여기다 3\n");

		 argv = (char**)malloc(sizeof(char*)*(argc+1));
		 
//		 token = strtok_r(file_name, " ", &ptr);
		 for(i = 0;i<argc ;i++)
		 {
				argv[i] = (char*)malloc(sizeof(char)*strlen(tempToks[i]));
				strlcpy(argv[i], tempToks[i], strlen(tempToks[i])+1); 

//				token = strtok_r(NULL, " ", &ptr);
		 }
		 argv[i] = NULL;
		 //printf("여기다4\n");

		for(i = 0;i<argc;i++)
		{
				//printf("%s\n", argv[i]);
		}
 }
//printf("넘사벽\n");
 for(i = 0;i<argc;i++)
		{
				//printf("%s\n", argv[i]);
		}
// if(argv[i] == NULL) printf("구일사의중심에서널을외치다\n");
  ///////////////////////////////
  //printf("111111111111111%s\n", myfn);
  /* Open executable file. */
  file = filesys_open (argv[0]);
  if (file == NULL) 
    {
      printf ("load: %s: open failed\n", argv[0]);
	  success = false;
      goto done; 
    }
 
  //printf("222222222222222222\n");
  /* Read and verify executable header. */
  if (file_read (file, &ehdr, sizeof ehdr) != sizeof ehdr
      || memcmp (ehdr.e_ident, "\177ELF\1\1\1", 7)
      || ehdr.e_type != 2
      || ehdr.e_machine != 3
      || ehdr.e_version != 1
      || ehdr.e_phentsize != sizeof (struct Elf32_Phdr)
      || ehdr.e_phnum > 1024) 
    {
      printf ("load: %s: error loading executable\n", file_name);
      goto done; 
    }

  //printf("3333333333333333\n");
  /* Read program headers. */
  //printf("!!!!!%d\n", ehdr.e_phoff);
  file_ofset = ehdr.e_phoff;
  for (i = 0; i < ehdr.e_phnum; i++) 
    {
      struct Elf32_Phdr phdr;

      if (file_ofset < 0 || file_ofset > file_length (file))
        goto done;
      file_seek (file, file_ofset);
		//printf("aaaaaaaaaaaaaa\n");
      if (file_read (file, &phdr, sizeof phdr) != sizeof phdr)
        goto done;
      file_ofset += sizeof phdr;
		//printf("aaaaaaaaaaaaaa\n");
      switch (phdr.p_type) 
        {
        case PT_NULL:
        case PT_NOTE:
        case PT_PHDR:
        case PT_STACK:
        default:
          /* Ignore this segment. */
          break;
        case PT_DYNAMIC:
        case PT_INTERP:
        case PT_SHLIB:
		//printf("bbbbbbbbbbb\n");
          goto done;
        case PT_LOAD:
          if (validate_segment (&phdr, file)) 
            {
              bool writable = (phdr.p_flags & PF_W) != 0;
              uint32_t file_page = phdr.p_offset & ~PGMASK;
              uint32_t mem_page = phdr.p_vaddr & ~PGMASK;
              uint32_t page_offset = phdr.p_vaddr & PGMASK;
              uint32_t read_bytes, zero_bytes;

			  if(mem_page == 0)
					  mem_page = 0x1000;
		//printf("ccccccccccccccc\n");
              if (phdr.p_filesz > 0)
                {
                  /* Normal segment.
                     Read initial part from disk and zero the rest. */
                  read_bytes = page_offset + phdr.p_filesz;
                  zero_bytes = (ROUND_UP (page_offset + phdr.p_memsz, PGSIZE)
                                - read_bytes);
                }
              else 
                {
                  /* Entirely zero.
                     Don't read anything from disk. */
                  read_bytes = 0;
                  zero_bytes = ROUND_UP (page_offset + phdr.p_memsz, PGSIZE);
                }
              if (!load_segment (file, file_page, (void *) mem_page,
                                 read_bytes, zero_bytes, writable))
                goto done;

		//printf("dddddddddddddd\n");
            }
          else
		  {
				  //printf("ggggggggggggggggggg\n");
            goto done;

		//printf("eeeeeeeeeee\n");
		  }
          break;
        }
    }

  //printf("4444444444444\n");
  /* Set up stack. */
  if (!setup_stack (esp))
    goto done;
  //printf("ready to set stack\n\n\n\n");

  {
		  int i, j, argvL, wa_cnt = 0;
		  char** memArgv = (char**)malloc(sizeof(char*)*(argc+1));

//		  memArgv = (char*)
		  //printf("c :: %d\n", argc);
		  for(i = 0;i<argc;i++)
		  {
				 	 argvL = strlen(argv[argc-i-1]);

		  		 	 //printf("l :: %d\n", argvL);
				 	 for(j = 0;j<=argvL;j++)
				 	 {
					 		 wa_cnt++;
							  memset(--(*esp), argv[argc-i-1][argvL-j], 1);
				  	 }

					 memArgv[argc-i-1] = *esp;
		  }
		  memArgv[i] = NULL;
		//word align
		  {

		  *esp -=(4- wa_cnt%4);
		memset(*esp, 0,4-wa_cnt%4);
		  }
		//address
//		printf("addr!!\n");
		 for(i = argc;i>=0;i--)
		 {
				 unsigned int addr;
				 if(memArgv[i] == NULL) addr = 0;
				 else addr = (int)memArgv[i];
				 {
					 unsigned int rem,i;
					 rem =addr;
				//	 printf("%x\n", rem);
		 			 for(i = 0 ; i<sizeof(addr) ; i++)
		 			 {
						
						memset(--(*esp),rem/(16777216),1 );
				//		printf("%x %d", rem, rem);
						rem %= 16777216;
						rem *= (16*16);

				//		printf("->%x %d\n", rem,rem);
 		  			  }
						 
				 }
			//	 printf("%x\n", (int)memArgv[i]);

		 }

		 {
			unsigned int addr = *esp;
			{
					 unsigned int rem,i;
					 rem =addr;
				//	 printf("%x\n", rem);
		 			 for(i = 0 ; i<sizeof(addr) ; i++)
		 			 {
						memset(--(*esp),rem/(16777216),1 );
				//		printf("%x %d", rem, rem);
						rem %= 16777216;
						rem *= (16*16);

				//		printf("->%x %d\n", rem,rem);
 		  			  }
						 
				 }
					
		 }


		  unsigned int rem;
		  rem = argc;
		  for(i = 0 ; i<sizeof(int) ; i++)
		  {
				memset(--(*esp),rem/(16777216),1 );
				//		printf("%x %d", rem, rem);
				rem %= 16777216;
				rem *= (16*16);
 		   }
		  *esp -= sizeof(void*);
		  memset(*esp, 0,sizeof(void *));
		
 	/*	 for(i = 0;i < argc; i++)
		  {
		  
		  }*/
  }
  //hex_dump(*esp, *esp, 108,1);



  /* Start address. */
  //printf("\n\n\n\n\n\nfefe recul\n");
  *eip = (void (*) (void)) ehdr.e_entry;
//printf("\n\n\n\n\n\nhere reculsive\n");
  success = true;

 done:
  /* We arrive here whether the load is successful or not. */
  //printf("im done\n\n");
  file_close (file);
  return success;
}

/* load() helpers. */

static bool install_page (void *upage, void *kpage, bool writable);

/* Checks whether PHDR describes a valid, loadable segment in
   FILE and returns true if so, false otherwise. */
static bool
validate_segment (const struct Elf32_Phdr *phdr, struct file *file) 
{
  /* p_offset and p_vaddr must have the same page offset. */
  if ((phdr->p_offset & PGMASK) != (phdr->p_vaddr & PGMASK))
  {
		  //printf("\n\n1^^\n");
    return false; 
  }

  /* p_offset must point within FILE. */
  if (phdr->p_offset > (Elf32_Off) file_length (file)) 
  {
		  //printf("\n\n2^^\n");
		  return false;
  }
  /* p_memsz must be at least as big as p_filesz. */
  if (phdr->p_memsz < phdr->p_filesz) 
  {
		  //printf("\n\n3^^\n");
		  return false; 
  }
  /* The segment must not be empty. */
  if (phdr->p_memsz == 0)
  {
		  //printf("\n\n4^^\n");
    return false;
  }
 /////여기다 
  /* The virtual memory region must both start and end within the
     user address space range. */
  if (!is_user_vaddr ((void *) phdr->p_vaddr))
  {
		  //printf("\n\n5^^\n");
    return false;
  }
	if (!is_user_vaddr ((void *) (phdr->p_vaddr + phdr->p_memsz)))
	{ 
		  //printf("\n\n6^^\n");
			return false;
	}
  /* The region cannot "wrap around" across the kernel virtual
     address space. */
  if (phdr->p_vaddr + phdr->p_memsz < phdr->p_vaddr)
  {
		  //printf("\n\n7^^\n");
		  return false;
  }

  /* Disallow mapping page 0.
     Not only is it a bad idea to map page 0, but if we allowed
     it then user code that passed a null pointer to system calls
     could quite likely panic the kernel by way of null pointer
     assertions in memcpy(), etc. */
  if (phdr->p_offset < PGSIZE)
  {
		  //printf("\n\n여기야^^\n");
		  return false;
  }

  /* It's okay. */
  return true;
}

/* Loads a segment starting at offset OFS in FILE at address
   UPAGE.  In total, READ_BYTES + ZERO_BYTES bytes of virtual
   memory are initialized, as follows:

        - READ_BYTES bytes at UPAGE must be read from FILE
          starting at offset OFS.

        - ZERO_BYTES bytes at UPAGE + READ_BYTES must be zeroed.

   The pages initialized by this function must be writable by the
   user process if WRITABLE is true, read-only otherwise.

   Return true if successful, false if a memory allocation error
   or disk read error occurs. */
static bool
load_segment (struct file *file, off_t ofs, uint8_t *upage,
              uint32_t read_bytes, uint32_t zero_bytes, bool writable) 
{
  ASSERT ((read_bytes + zero_bytes) % PGSIZE == 0);
  ASSERT (pg_ofs (upage) == 0);
  ASSERT (ofs % PGSIZE == 0);

  file_seek (file, ofs);
  while (read_bytes > 0 || zero_bytes > 0) 
    {
      /* Calculate how to fill this page.
         We will read PAGE_READ_BYTES bytes from FILE
         and zero the final PAGE_ZERO_BYTES bytes. */
      size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
      size_t page_zero_bytes = PGSIZE - page_read_bytes;

      /* Get a page of memory. */
      uint8_t *knpage = palloc_get_page (PAL_USER);
      if (knpage == NULL)
        return false;

      /* Load this page. */
      if (file_read (file, knpage, page_read_bytes) != (int) page_read_bytes)
        {
          palloc_free_page (knpage);
          return false; 
        }
      memset (knpage + page_read_bytes, 0, page_zero_bytes);

      /* Add the page to the process's address space. */
      if (!install_page (upage, knpage, writable)) 
        {
          palloc_free_page (knpage);
          return false; 
        }

      /* Advance. */
      read_bytes -= page_read_bytes;
      zero_bytes -= page_zero_bytes;
      upage += PGSIZE;
    }
  return true;
}

/* Create a minimal stack by mapping a zeroed page at the top of
   user virtual memory. */
static bool
setup_stack (void **esp) 
{
  uint8_t *kpage;
  bool success = false;

  kpage = palloc_get_page (PAL_USER | PAL_ZERO);
  if (kpage != NULL) 
    {
      success = install_page (((uint8_t *) PHYS_BASE) - PGSIZE, kpage, true);
      if (success)
        *esp = PHYS_BASE;
      else
        palloc_free_page (kpage);
    }
  return success;
}

/* Adds a mapping from user virtual address UPAGE to kernel
   virtual address KPAGE to the page table.
   If WRITABLE is true, the user process may modify the page;
   otherwise, it is read-only.
   UPAGE must not already be mapped.
   KPAGE should probably be a page obtained from the user pool
   with palloc_get_page().
   Returns true on success, false if UPAGE is already mapped or
   if memory allocation fails. */
static bool
install_page (void *upage, void *kpage, bool writable)
{
  struct thread *th = thread_current ();

  /* Verify that there's not already a page at that virtual
     address, then map our page there. */
  return (pagedir_get_page (th->pagedir, upage) == NULL
          && pagedir_set_page (th->pagedir, upage, kpage, writable));
}
