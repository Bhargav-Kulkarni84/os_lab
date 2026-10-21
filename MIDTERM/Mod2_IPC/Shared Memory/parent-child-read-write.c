#include<stdio.h>
#include<stdlib.h> 
#include<unistd.h> 
#include<string.h> 
#include<wait.h>
#include<sys/shm.h> 
#include<sys/ipc.h> 

#define size 1024

int main(){

    //Create a key
    key_t key = ftok("shared",1);

    //Get a shared memory id
    int shmid = shmget(key,size,IPC_CREAT|0666);

    //Attach shared memory
    char* shared = shmat(shmid,NULL,0);

    pid_t pid = fork();

    if(pid==0){
        sleep(2);
        printf("Child reads %s \n",shared);
        shmdt(shared);
        exit(0);
    }

    else{
        printf("Parent writes\n");
        strcpy(shared,"Hello from parent\n");
        wait(NULL);
        shmdt(shared);
        shmctl(shmid,IPC_RMID,NULL);
        exit(0);
    }

    return 0;
}