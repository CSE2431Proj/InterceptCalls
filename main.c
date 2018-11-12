#include "headers.h"

int main() {

    // initialize bounded buffer
    shared_struct *ptr = (shared_struct *) malloc(sizeof(shared_struct));
    ptr->in = 0;
    ptr->out = 0;
    sem_init(&ptr->mutex, 0, 1);
    sem_init(&ptr->full,0,0);
    sem_init(&ptr->empty,0,BUFFER_SIZE);//initialize empty to size of buffer


    // initialize pthread args
    pthread_args *p_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_args->ptr = ptr;
    p_args->fd = input;

    pthread_args *c_args = (pthread_args *) malloc(sizeof(pthread_args));
    c_args->ptr = ptr;
    c_args->fd = output;

    pthread_attr_t attr;            // set of thread attributes
    pthread_attr_init(&attr);       // get the default attributes

    pthread_t producer_t;
    pthread_t consumer_t;

    // create producer and consumer threads
    pthread_create(&producer_t, &attr, producer, p_args);
    pthread_create(&consumer_t, &attr, consumer, c_args);

    // wait for working threads
    pthread_join(consumer_t, NULL);
    pthread_join(producer_t, NULL);

    // close open files

    // free
    free(p_args);
    free(c_args);
    free(ptr);

    exit(0);
}
