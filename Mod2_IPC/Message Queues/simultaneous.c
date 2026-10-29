#include<stdio.h>
#include<fcntl.h>
#include<mqueue.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    
    struct mq_attr attr = {0};
    attr.mq_msgsize = 128;
    attr.mq_maxmsg = 10;

    bool isSender = strcmp("sender",argv[1]) == 0;

    if(isSender){
        mqd_t mq = mq_open("/t",O_CREAT|O_WRONLY,0600,&attr);

        while(true){

            printf("Enter message \n");
            char msg[128];
            scanf("%s",msg);
            mq_send(mq,msg,strlen(msg),0);

            if(strcmp("exit",msg) == 0) break;

        }


    }
    
    else{
        mqd_t mq = mq_open("/t",O_CREAT|O_RDONLY,0600,&attr);

        while(true){
            
            char buffer[129];
            int n = mq_receive(mq,buffer,sizeof(buffer),0);
            
            if (n >= 0) {
                buffer[n] = '\0';
                printf("Sender sent = %s\n", buffer);

                if(strcmp("exit",buffer) == 0) break;
            }


        }

        mq_close(mq);
        mq_unlink("/temp");

    }


    return 0;
}