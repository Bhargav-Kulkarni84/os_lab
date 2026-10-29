#include<stdio.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<stdbool.h>
#include<unistd.h>

#define N 5

typedef struct Shared {

    sem_t full;
    sem_t empty;
    sem_t mutex;
    int in;
    int out;
    // pthread_mutex_t mutex;

    char buffer[N][100];

}Shared;

int main(int argc, char* argv[]){

    if(strcmp(argv[1],"init")==0){
        int fd = shm_open("/shared",O_CREAT|O_RDWR,0600);
        ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

        shared->in = 0;
        shared->out = 0;

        sem_init(&shared->full,1,0);
        sem_init(&shared->empty,1,N);
        sem_init(&shared->mutex,1,1);
        // pthread_mutex_init(&shared->mutex,NULL);
        close(fd);
        munmap(shared,sizeof(Shared));

        return 0;
    }

    if(strcmp(argv[1],"producer")==0){

        int fd = shm_open("/shared",O_CREAT|O_RDWR,0600);
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
        
        while(true){

            char buffer[100];
            printf("Enter element to be produced \n");
            scanf("%s",buffer);
            
            //Reduce the number of available slot
            sem_wait(&shared->empty);
                //Lock critical section
                sem_wait(&shared->mutex);   
            
                    int in = shared->in;
                    strcpy(shared->buffer[in],buffer);
                    shared->in = (shared->in+1)%N;

                //Unlock critical section.
                sem_post(&shared->mutex);   
            //Signal cosumer that atleast 1 product is produced.
            sem_post(&shared->full);

            if(strcmp(buffer,"exit") == 0) break;
        }

        close(fd);
        munmap(shared,sizeof(Shared));

    }

    else{

        char buffer[100];

        int fd = shm_open("/shared",O_CREAT|O_RDWR,0600);
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
        
        while(true){

            //Consume 1 product of off avaialble ones.
            sem_wait(&shared->full);
            sem_wait(&shared->mutex);
            
                    int out = shared->out;
                    printf("Consumed item - %d message = %s \n",out+1,shared->buffer[out]);    
                    strcpy(buffer,shared->buffer[out]);
                    shared->out = (shared->out+1)%N;
                    
                sem_post(&shared->mutex);
            //Signal producer that atleast 1 slot is empty.
            sem_post(&shared->empty);

            if(strcmp(buffer,"exit") == 0) break;
        }

        close(fd);
        munmap(shared,sizeof(Shared));

    }

    return 0;
}