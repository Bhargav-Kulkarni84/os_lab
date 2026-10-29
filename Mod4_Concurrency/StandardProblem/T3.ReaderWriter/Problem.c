#include<stdio.h>
#include<pthread.h>

char message [100];

void* read(void* args){

    int id = *(int*)args;

    for(int i=0; i<5; i++){
        // snprintf(message,sizeof(message),"Read by thread-%d",id);
        printf("Read by thread-%d\n",id);
    }

}

void* write(void* args){

    int id = *(int*)args;

    for(int i=0; i<5; i++){
        // snprintf(message,sizeof(message),"Written by thread-%d",id);
        // printf("%s\n",message);
        printf("Written by thread-%d\n",id);
    }

}

int main(){

    pthread_t t1,t2,t3,t4;

    int id1 = 1;
    int id2 = 2;
    int id3 = 3;
    int id4 =4 ;

    pthread_create(&t1,NULL,read,&id1);
    pthread_create(&t2,NULL,read,&id2);
    pthread_create(&t3,NULL,write,&id3);
    pthread_create(&t4,NULL,write,&id4);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);

    return 0;

}