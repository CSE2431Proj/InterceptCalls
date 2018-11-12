/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

typedef struct {
    int id;                         // item id
    char data[256];                 // item data
} item;

typedef struct {
    int in;                         // shared variable in
    int out;                        // shared variable out
    item buffer[BUFFER_SIZE];       // bounded buffer with size BUFFER_SIZE
    sem_t mutex;			    
    sem_t full;	
    sem_t empty;
} shared_struct;

typedef struct {
    shared_struct *ptr;             // shared by producer threads and consumer threads
    		                       // file descriptor of input file or output file
} pthread_args;

void *producer(void *args);         // producer thread
void *consumer(void *args);         // consumer thread
