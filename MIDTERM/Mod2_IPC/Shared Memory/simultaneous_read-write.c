#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>

typedef struct Shared{
    char text[256];
    sem_t read;
    sem_t write;
} Shared;

int main(int argc , char* argv[]){

    int writer = strcmp(argv[1],"writer") == 0;
    int fd;

    if(writer){

        fd = shm_open("/simultaneous",O_CREAT|O_RDWR,0666);
        ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
        close(fd);

        printf("Press exit to end\n");

        sem_init(&shared->write,1,0);
        sem_init(&shared->read,1,1);

        while(true){

            char msg [100];
            printf("Write a message to send to reader\n");
            scanf("%s",msg);
            
            //Wait for reader or lock the reader
            sem_wait(&shared->read);
                //Critical Section
                strcpy(shared->text,msg);
            //Release the reader
            sem_post(&shared->write);

            if(strcmp(msg,"exit") == 0) break;

        }

        //Wait for the reader to consume the exit message;
        sem_wait(&shared->read);

        //Remove the semaphores
        sem_destroy(&shared->read);
        sem_destroy(&shared->write);

        //Detach the shared memory
        munmap(shared,256);

        //Delete the shared memeory
        shm_unlink("/simultaneous");

    }
    else{

        fd = shm_open("/simultaneous",O_RDWR,0666);
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
        close(fd);
        
        while(true){
            
            //Receive the message from the shared memory if writer allows
            //Lock the writer
            sem_wait(&shared->write);
                printf("%s\n",shared->text);
            sem_post(&shared->read);

            if(strcmp(shared->text,"exit")==0) break;

        }

        //Detach the shared memory
        munmap(shared,256);

    }


}