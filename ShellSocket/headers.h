/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include <semaphore.h>
#include<unistd.h>
#define BUFFER_SIZE 1
//See chapter 3 for makeFIle slide 45
typedef struct {                    // item id
    char data[100];                 // item data
} command;

typedef struct {
    int in;                         // shared variable in
    int out;                        // shared variable out
    command buffer[BUFFER_SIZE];       // bounded buffer with size BUFFER_SIZE  
} shared_struct;

/*typedef struct {
    int port;
    shared_struct *ptr;             // shared by producer threads and consumer threads
    		                       // file descriptor of input file or output file
} input_args;*/

void *producer(void *args);         // producer thread
void *consumer(void *args);         // consumer thread
