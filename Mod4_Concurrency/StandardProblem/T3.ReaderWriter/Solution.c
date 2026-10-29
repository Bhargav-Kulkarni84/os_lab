/*
    Reader should not read before writer writing
    More than 1 writer should not write to a file simultaneously. (Mututal exclusion between write)
    READ AND WRITE TO SAME FILE ==> MUTUALLY EXCUSIVE.
*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

char message [100];

sem_t write;
sem_t mutex;
int readCount;

void* read(void* args){

    //Increment the no of readers.
    sem_wait(&mutex);
        readCount++;

        if(readCount == 1){
            sem_wait(&write);
        }

    sem_post(&mutex);

    int id = *(int*)args;
    for(int i=0; i<5; i++){
        printf("Reader-%d is reading\n",id);
    }
    printf("Read by reader-%d\n",id);

    //Decrement the no of readers.
    sem_wait(&mutex);
        readCount--;

        if(readCount == 0){
            //release writer lock.
            sem_post(&write);
        }
    sem_post(&mutex);

}

void* write(void* args){

    //Check if any writer is already writing || any reader reading.
    sem_wait(&write);

        int id = *(int*)args;
        for(int i=0; i<5; i++){
            printf("Writer-%d is writing\n",id);
        }
        printf("Written by writer-%d\n",id);

    sem_post(&write);

}

int main(){

    pthread_t t1,t2,t3,t4;

    int id1 = 1;
    int id2 = 2;
    int id3 = 3;
    int id4 = 4;

    readCount = 0;
    sem_init(&write,0,1);
    sem_init(&mutex,0,1);

    pthread_create(&t1,NULL,read,&id1);
    pthread_create(&t2,NULL,read,&id2);
    pthread_create(&t3,NULL,write,&id3);
    pthread_create(&t4,NULL,write,&id4);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);

    sem_destroy(&write);
    sem_destroy(&mutex);

    return 0;

}

/*

    readers;
    write;
    mutex;

    do{
        write(){
            wait(write);
                {CS}
            signal(write);
        }
    }
    while(true)

    do{
        read(){
    
            wait(mutex)
                
                readers++;
                if(readers == 1){
                    wait(write)
                }

            signal(mutex)
    
                {CS}
            
            wait(mutex)

                readers--
                if(reades == 0){
                    signal(write)    
                }
    
            signal(mutex)
    
        }
    }
    while(true)

*/