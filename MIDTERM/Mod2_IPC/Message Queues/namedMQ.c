// Named ==> Two independent processes use the same key
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct Message{
    long mtype;
    char mtext[100];
} Message;

int main(int argc, char *argv[]){

    int result = strcmp(argv[1],"send");

    int sender = 0;
    if(result == 0) sender = 1;

    key_t key = ftok("key",1);

    //Create or Access message queues
    int msqid;
    if(sender) msqid = msgget(key,IPC_CREAT|0666);
    else msqid = msgget(key,0666);
    
    if(sender){

        Message msg;

        //2.Construct Message.
        msg.mtype = 1;
        strcpy(msg.mtext,"Hello from named message queue");

        //3.Send Message.
        msgsnd(msqid,&msg,strleng(msg.mtext)-1,0);

        printf("Sender Message Send \n");

        msgctl(msqid,IPC_RMID,NULL);

    }
    else{

        Message msg;

        //3.Receive message with type 1
        msgrcv(msqid,&msg,sizeof(msg.mtext),1,0);
        printf("Receiver: %s\n", msg.mtext);
        msgctl(msqid,IPC_RMID,NULL);


    }

    return 0;

}
