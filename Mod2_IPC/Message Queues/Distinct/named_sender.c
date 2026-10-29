#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<mqueue.h>
#include<sys/stat.h>

int main(){

    struct mq_attr attr = {0};
    
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 128;

    //Create/open the named message queue
    mqd_t mq = mq_open("/temp",O_CREAT|O_WRONLY,0600,&attr);
    
    char message [] = "Hello from parent\n";
    mq_send(mq,message,strlen(message)+1,0);

    printf("Message sent successfully\n");

    mq_close(mq);   

    return 0;

}