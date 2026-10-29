/*
    Producer should not produce if buffer is full.
    Consumer should not consume if buffer is empty.

    Consumer should not consume from the resouce if it is not produced by the consumer ==> Synchronization

*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define N 5
int buffer [N];

int in,out;

sem_t empty,full,mutex;

void* producer(void* args){

    for(int i=1; i<=10; i++){

        /*
            Check if there is any empty slot present
            empty == 0  all slots are filled
            empty > 0 ==> N-empty slots present
        */
        sem_wait(&empty);

        //Lock the buffer
        sem_wait(&mutex);

            buffer[in] = i;
            printf("Produced = %i\n",i);
            in = (in+1)%N;

        //Unlock the buffer.
        sem_post(&mutex);

        /*
            Increase the count of available resources in the buffer for consumer to produce.
            full = full + 1;
        */
        sem_post(&full);
        
    }
    
}

void* consumer(void* args){

    for(int i=1; i<=10; i++){

        /*
            Check if there are slots present
            full == 0  No available slot
            full > 0 ==> available slots present
        */
        sem_wait(&full);

        sem_wait(&mutex);

            int val = buffer[out];
            printf("Consumed = %d\n",val);
            out = (out+1)%N;

        sem_post(&mutex);

        /*Increment the no of empty slots*/
        sem_post(&empty);

    }


}

int main(){

    pthread_t t1,t2;

    in = 0;
    out = 0;

    pthread_create(&t1,NULL,producer,NULL);
    pthread_create(&t2,NULL,consumer,NULL);

    sem_init(&empty,0,5);
    sem_init(&full,0,0);
    sem_init(&mutex,0,1);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;

}

/*  
    mutex;
    empty = N;
    full = 0;


    do{

        producer(){

            wait(empty)
                wait(mutex)
                    {cs}
                signal(mutex)
            signal(full)

        }

    }
    while(true)


    do{

        consumer(){

            wait(full)
                wait(mutex)
                    {cs}
                signal(mutex)
            signal(empty)
        }

    }
    while(true)


*/