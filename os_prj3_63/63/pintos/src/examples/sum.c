/* cat.c

   Prints files specified on command line to the console. */

#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
int
main (int argc, char *argv[]) 
{
   printf("%d ", fibonacci (atoi(argv[1])));
   printf("%d\n", sum_of_four_integers(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4])));
//	printf("system caoo test\n");
  return EXIT_SUCCESS;
}
