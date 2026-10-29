#include<fcntl.h>
#include<sys/mman.h>
#include<semaphore.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<wait.h>
#include<unistd.h>
#include<stdio.h>

typedef struct Shared{
    char text[100];
    sem_t read;
    sem_t write;
}Shared;

int main(){

    // int fd = sem_open('/semaphores',O_CREAT|O_RDWR,0666);
    // ftruncate(fd,256);

    Shared* shared = mmap(NULL,sizeof(Shared),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

    sem_init(&shared->read,1,1);
    sem_init(&shared->write,1,0);

    pid_t pid = fork();

    if(pid == 0){

        while(true){

            char msg[] = "Reader Reads :";

            //READER
            sem_wait(&shared->write);
                strcat(msg,shared->text);
                printf("%s\n",msg);
            sem_post(&shared->read);

            if(strcmp(shared->text,"exit")==0) break;

        }

        munmap(shared,sizeof(Shared));
        return 0;

    }   

    while(1){

        printf("Enter string for reader to read\n");
    
        char msg[100];
        int n = read(0,msg,sizeof(msg));

        //WRITER
        sem_wait(&shared->read);
            strcpy(shared->text,msg);
        sem_post(&shared->write);

        if(strcmp(shared->text,"exit\n")==0) break;
    }

    wait(NULL);
    sem_destroy(&shared->read);
    sem_destroy(&shared->write);
    munmap(shared,sizeof(Shared));

    return 0;
}

   