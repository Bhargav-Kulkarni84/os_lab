#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<string.h>
#include<stdbool.h>
#include<mqueue.h>
#include<fcntl.h>

#define N 5


int main(int argc, char* argv []){
    
    //Check if the current program is producer or consumer.
    bool isProducer = strcmp("producer",argv[1]) == 0;
    
    if(isProducer){
        
        struct mq_attr attr = {0};
        attr.mq_msgsize = 128;
        attr.mq_maxmsg = N;
        
        mqd_t mq = mq_open("/pcmq",O_CREAT|O_RDWR,0600,&attr);
        
        while(true){
            
            char buffer[129];
            
            printf("Produced the element\n");
            scanf("%s",buffer);

            //Send the message.
            mq_send(mq,buffer,strlen(buffer)+1,0);
            if(strcmp(buffer,"exit")==0) break;
        }

    }

    else{

        struct mq_attr attr = {0};
        attr.mq_msgsize = 128;
        attr.mq_maxmsg = N;
        mqd_t mq = mq_open("/pcmq",O_CREAT|O_RDONLY,0600,&attr);

        while(true){

            char buffer[129];

            int n = mq_receive(mq,buffer,sizeof(buffer)-1,NULL);
            buffer[n] = '\0';
            printf("Consumed %s\n",buffer);
            if(strcmp(buffer,"exit")==0) break;

        }

        mq_close(mq);
        mq_unlink("/pcmq");

    }

}