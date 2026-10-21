#include<stdio.h>
#include<pthread.h>
#include<stdbool.h>

int turn;
bool flag[2];

int p1 = 0;
int p2 = 1;

int counter = 0;

void * increment(void * args){

    int id = *((int*)args);
    printf("Inside thread -%d\n",id);
    
    for(int i=0; i<100000; i++){
        
        flag[p1] = true;
        turn = p2;

        while(flag[p2] == true && turn==p2);

        //Critical Section
        counter++;

        flag[p1] = false;

    } 

    pthread_exit("Thread-1 Completed Excution");
}


void * decrement(void * args){

    int id = *((int*)args);
    printf("Inside thread -%d\n",id);
    
    for(int i=0; i<100000; i++){
        
        flag[p2] = true;
        turn = p1;

        while(flag[p1] && turn == p1);

        //Critical Section
        counter--;

        flag[p2] = false;

    }

    pthread_exit("Thread-2 Completed Excution");

}

int main(){

    turn = 0;
    flag[0] = false;
    flag[1] = false;

    pthread_t t1,t2;

    int id1 = 1;
    int id2 = 2;

    pthread_create(&t1,NULL,increment,&id1); 
    pthread_create(&t2,NULL,decrement,&id2); 

    void * s1;
    void * s2;

    pthread_join(t1,&s1);
    pthread_join(t2,&s2);

    printf("Return value from thread-1: %s\n", (char *)s1);
    printf("Return value from thread-2: %s\n", (char *)s2);

    printf("Expected Counter Value = %d\n",0);
    printf("Final Counter Value = %d\n",counter);

    return 0;

}

/*

    PSEUDOCODE

    turn = 0;
    flag = {false,false};

    flag[i] = true,
    turn = j

    while(flag[j] && turn == j);

    {CS}

    flag[j] = false;

*/