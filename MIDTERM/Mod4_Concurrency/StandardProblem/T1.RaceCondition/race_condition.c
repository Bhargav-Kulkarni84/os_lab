#include<stdio.h>
#include<pthread.h>

int counter = 0;

void * increment(void * args){
    int id = *((int*)args);
    printf("Inside process-%d \n",id);
    for(int i=0; i<10000000; i++) counter++;

    pthread_exit("Thread1 Completed Excution");
}

void * decrement(void * args){
    int id = *((int*) args);
    printf("Inside process-%d \n",id);
    for(int i=0; i<10000000; i++) counter--;

    pthread_exit("Thread2 Completed Excution");
}

int main(){

    pthread_t t1, t2;

    int id1 = 1;
    int id2 = 2;

    pthread_create(&t1,NULL,increment,&id1);
    pthread_create(&t2,NULL,decrement,&id2);

    void* s1 ;
    void* s2 ;

    pthread_join(t1,&s1);
    pthread_join(t2,&s2);

    printf("Return value from thread-1: %s\n", (char *)s1);
    printf("Return value from thread-2: %s\n", (char *)s2);

    printf("Expected Counter Value = %d\n",0);
    printf("Final Counter Value = %d\n",counter);

    return 0;

}