#include<stdio.h>
#include<pthread.h>

#define N 5
int buffer [N];

int in;
int out;

void* producer(void* args){

    for(int i=1; i<=10; i++){
        buffer[in] = i;
        printf("Produced = %i\n",i);
        in = (in+1)%N;
    }

}

void* consumer(void* args){

    for(int i=1; i<=10; i++){
        int val = buffer[out];
        printf("Consumed = %d\n",val);
        out = (out+1)%N;
    }

}

int main(){

    pthread_t t1,t2;

    in = 0;
    out = 0;

    pthread_create(&t1,NULL,producer,NULL);
    pthread_create(&t1,NULL,consumer,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return 0;

}