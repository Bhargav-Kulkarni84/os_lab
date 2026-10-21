#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<string.h>

#define size 1024

typedef struct Message{

    long mtype;
    char mtext[size];

} Message;

int main(int argc, char* argv[]){

    int sender = (strcmp(argv[1],"sender") == 0);

    int key = ftok("mq",1);

    int msqid;
    if(sender) msqid = msgget(key,IPC_CREAT|0666);
    else msqid = msgget(key,0666);

    if(sender){
        Message msg;

        msg.mtype = 1;
        strcpy(msg.mtext,"Message from sender");

        msgsnd(msqid,&msg,sizeof(msg.mtext)-1,0);
        printf("Message Sent\n");
    }

    else{
        Message msg;
        msgrcv(msqid,&msg,sizeof(msg.mtext)+1,1,0);
        printf("Message Received : %s\n",msg.mtext);
    }

}