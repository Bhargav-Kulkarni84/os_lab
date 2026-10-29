#include<stdio.h>
#include<semaphore.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<string.h>

typedef struct Shared{

    int readCount;
    sem_t mutex;
    sem_t write;

} Shared;

int main(int argc, char* argv[]){

    if(strcmp(argv[1],"init") == 0){

        int fd = shm_open("/rdwr",O_CREAT|O_RDWR,0600);
        ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

        shared->readCount = 0;
        sem_init(&shared->mutex,1,1);
        sem_init(&shared->write,1,1);

        munmap(shared,sizeof(Shared));
        close(fd);

        return 0;
        
    }

    if(strcmp(argv[1],"writer") == 0){

        int fd = shm_open("/rdwr",O_CREAT|O_RDWR,0600);
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

        int file = open("file.txt",O_CREAT|O_RDWR,0600);

        //Writer programs
        while(true){

            //Lock the writers
            printf("Write a message to send \n");
            char msg[100];
            // scanf("%s",msg);
            read(0,msg,sizeof(msg));

            printf("Writer waiting for lock...\n");

            sem_wait(&shared->write);
                printf("Writer acquired lock. Writing...\n");
                write(file,msg,strlen(msg));
                sleep(5);
                printf("Writer releasing lock\n");
            sem_post(&shared->write);

            if(strcmp(msg,"exit") == 0) break;

        }

        munmap(shared,sizeof(Shared));;
        close(file);
        
    }

    else{

        int fd = shm_open("/rdwr",O_CREAT|O_RDWR,0600);
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

        int file = open("file.txt",O_CREAT|O_RDWR,0600);

        //Reader Program
        while(true){

            printf("Reader PID %ld wants to read\n",(long)getpid());
            
            sem_wait(&shared->mutex);
                shared->readCount++;
                if(shared->readCount==1) sem_wait(&shared->write);
                printf("Reader PID %ld entered. Active readers: %d\n",(long)getpid(), shared->readCount);
            sem_post(&shared->mutex);
            
            char msg[100];

            //Read message from file
            int n = read(file,msg,sizeof(msg)-1);
            msg[n] = '\0';

            if(n>0){
                printf("Reader PID %ld reads: %s\n",(long)getpid(), msg);
                sleep(5); 
            }

            //Reader completes reading
            //decrease no of readers
            sem_wait(&shared->mutex);
                shared->readCount--;
                if(shared->readCount == 0) sem_post(&shared->write);
                printf("Reader PID %ld finished. Active readers: %d\n",(long)getpid(), shared->readCount);
            sem_post(&shared->mutex);


            if(strcmp(msg,"exit") == 0) break;

            sleep(1);


        }

        munmap(shared,sizeof(Shared));
        close(file);

    }


}