#include<stdio.h>
#include<stdbool.h>
#include<mqueue.h>
#include<fcntl.h>

int main(){

    struct mq_attr attr = {0};
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;

    mqd_t mq = mq_open("/temp",O_CREAT|O_RDONLY,0600,&attr);

    char buffer[100];
    int priority;

    printf("Waiting for message...\n");

    int n = mq_receive(mq,buffer,sizeof(buffer),&priority);

    printf("Received: %s\n", buffer);
    printf("Priority: %u\n", priority);

    mq_close(mq);

    mq_unlink("/temp");

    return 0;

}