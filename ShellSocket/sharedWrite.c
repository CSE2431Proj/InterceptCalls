#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include"headers.h"

   const char *name = "Shared Mem";
   int shm_fd;		// file descriptor, from shm_open()
   char *ptr;	// base address, from mmap()
   char* buf;
   long size;

int main() 
{ 
 buf = "ls";
//Retrieve Shared Memory
	
		/* create the shared memory segment */
        shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
        if (shm_fd == -1) {
            printf("Shared memory failed\n");
            exit(-1);
        }   

  /* configure the size of the shared memory segment */
        size = 4096;
        ftruncate(shm_fd, size);   

        /* now map the shared memory segment in the address space of the process */
        ptr = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            printf("Map failed\n");
            exit(-1);
        }

	//write data
	//sprintf(ptr,"%s", buf);
        printf("%s", ptr);

      //  shm_unlink(name);
	
}
