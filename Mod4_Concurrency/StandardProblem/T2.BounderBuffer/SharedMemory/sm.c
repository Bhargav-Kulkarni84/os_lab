#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<string.h>
#include<stdbool.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>

#define N 5

typedef struct Shared{
    char buffer [N][100]; 
    int in;
    int out;
    // pthread_mutex_t mutex;
    // sem_t full;
    // sem_t empty;
} Shared;

int main(int argc, char* argv []){

    //Check if the current program is initialization, producer or consumer.
    if(strcmp(argv[1],"init") == 0){

        int fd = shm_open("/shared",O_CREAT|O_RDWR,0600);
        ftruncate(fd,sizeof(Shared));

        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
        shared->in = 0;
        shared->out = 0;
        // sem_init(&(shared->full),1,0);
        // sem_init(&(shared->empty),1,N);
        // pthread_mutex_init(&(shared->mutex),NULL);

        munmap(shared, sizeof(Shared));
        close(fd);

        return 0;
    };

    bool isProducer = strcmp("producer",argv[1]) == 0;

    if(isProducer){

        int fd = shm_open("/shared",O_RDWR,0600);
        // ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

        while(true){

            int in = (shared->in)%N;
            
            printf("Produced the %d th element\n",in+1);

            char msg[100];
            scanf("%s",msg);
            strcpy(shared->buffer[in],msg);

            if(strcmp(shared->buffer[in],"exit")==0) break;

            //Update the start index.
            shared->in = (shared->in+1)%N;

        }

        close(fd);
        munmap(shared,sizeof(Shared));

    }

    else{

        int fd = shm_open("/shared",O_RDWR,0600);
        // ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

        while(true){

            while(shared->in == shared->out){};

            int out = (shared->out)%N;

            char msg[100];
            strcpy(msg,shared->buffer[out]);
            
            printf("Consumed the %d th element\n",out+1);
            printf("Producer Produced = %s\n",msg);
            
            if(strcmp(shared->buffer[out],"exit")==0) break;

            //Update out index.
            shared->out = (shared->out+1)%N;

        }

        close(fd);
        munmap(shared,sizeof(Shared));

    }

}