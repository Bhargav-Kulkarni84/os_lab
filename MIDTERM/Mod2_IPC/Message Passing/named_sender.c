#include<mqueue.h>
#include<string.h>
#include<fcntl.h>
#include<stdio.h>

int main(){

    struct mq_attr attr;

    mqd_t mq = mq_open("/temp",O_WRONLY);
    
    char message [] = "Hello from parent\n";
    mq_send(mq,message,strlen(message)+1,5);

    printf("Message sent successfully\n");

    mq_close(mq);   

    return 0;

}