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
    int shmid = shmget(key,1024,IPC_CREAT|0666);

    //Attach the shared memory address using id.
    char* shared = shmat(shmid,NULL,0);

    //Writer writes to the shard memory.
    char msg1[] = "Writer writing to shared memory\n";
    write(1,msg1,sizeof(msg1)-1);
    
    char msg2[] = "Write a message for reader\n";
    write(1,msg2,sizeof(msg2)-1);
    
    char msg3[1024];
    int n = read(0,msg3,sizeof(msg3)-1);
    
    msg3[n] = '\0';
    strcpy(shared,msg3);

    //Detach writer from shared memory
    shmdt(shared);

    return 0;

}