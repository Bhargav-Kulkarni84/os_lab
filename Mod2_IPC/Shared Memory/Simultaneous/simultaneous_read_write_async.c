#include<stdio.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<string.h>

typedef struct Shared{
    char message[128];
}Shared;

int main(int argc, char* argv []){

    bool isWriter = strcmp("writer",argv[1]) == 0;

    if(isWriter){

        int fd = shm_open("/shared",O_CREAT|O_RDWR,0666);
        ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

        while(true){

            char buffer[100];
    
            printf("Enter a message to be sent\n");
            scanf("%s",buffer);
    
            strcpy(shared->message,buffer);
    
            if(strcmp(shared->message ,"exit") == 0) break;
        }

    }

    else{

        int fd = shm_open("/shared",O_CREAT|O_RDWR,0666);
        ftruncate(fd,sizeof(Shared));
        Shared* shared = mmap(NULL,sizeof(Shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

        if(strlen(shared->message) == 0) printf("Waiting for the message from sender\n");

        while(true){
            
            while(strlen(shared->message) == 0){};

            printf("Receiver Received = %s \n",shared->message);    
            if(strcmp(shared->message ,"exit") == 0) break;
            strcpy(shared->message,"\0");

        }
        
        munmap(shared,sizeof(Shared));
        close(fd);

    }

}