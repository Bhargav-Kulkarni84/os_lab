#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<string.h>

//Writer
int main(){

    //Create a unique key for shared memeory
    key_t key = ftok("shared",0);

    //Get the shared memory id.
    // int shmid = shmget(key,1024,IPC_CREAT|0666);
    int shmid = shmget(key,1024,0);

    if(shmid == -1){
        char msg2[] = "Nothing written by writer \n";
        write(1,msg2,sizeof(msg2)-1);
        return 0;
    }

    //Attach the shared memory address using id.
    // char* shared = shmat(shmid,NULL,0);
    char* shared = shmat(shmid,NULL,0);

    //Reader reads from the shared memory.
    char msg1[] = "Reader read's \n";
    write(1,msg1,sizeof(msg1)-1);

    write(1,shared,strlen(shared));

    //Detach writer from shared memory
    shmdt(shared);

    //Delete the shared memory
    shmctl(shmid,IPC_RMID,0);

}