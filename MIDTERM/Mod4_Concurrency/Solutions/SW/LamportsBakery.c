#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>
#include<math.h>
#define N 5

bool choosing[N];
int nums[N];

int counter = 0;

void* increment(void * args){

    int id = *((int*)args);

    printf("Inside thread-%d\n",id);

    //Choose the process token number
    choosing[id] = true;

    int maxToken = 0;
    for(int k=0; k<N; k++) maxToken = fmax(maxToken,nums[k]);
    nums[id] = 1 + maxToken;

    choosing[id] = false;

    //Give the processes with the smaller token number chance to complete.
    for(int j=0; j<N; j++){
        
        //Wait if jth process is still choosing its token.
        while(choosing[j]);

        //Wait if previous process with a smaller token no is still has to complete.
        while(nums[j]!=0 && ((
                nums[j]<nums[id] || 
                nums[j] == nums[id] && j<id)
            ));
        
    }

    //Critical Section
    for(int i=0; i<1000000; i++){        
        counter++;
    }  

    nums[id] = 0;
    
}

int main(){

    for(int i=0; i<N; i++) {
        choosing[i] = false;
        nums[i] = 0;
    }

    pthread_t t1,t2,t3,t4,t5;

    int id1 = 0;
    int id2 = 1;
    int id3 = 2;
    int id4 = 3;
    int id5 = 4;

    pthread_create(&t1,NULL,increment,&id1);
    pthread_create(&t2,NULL,increment,&id2);
    pthread_create(&t3,NULL,increment,&id3);
    pthread_create(&t4,NULL,increment,&id4);
    pthread_create(&t5,NULL,increment,&id5);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    pthread_join(t5,NULL);

    printf("Expected Value = 500000\n");
    printf("Actual Value = %d\n",counter);

    return 0;
}

/*

    choosing[i] = true;
    nums[i] = 1 + max(nums[0]....nums[N])
    choosing[i] = false;

    for(int j=0; j<N; j++){
    
        while(choosing[j]);
        while(j!=0 && (nums[j],j) < (nums[i],i));

    }

    {CS}

    nums[i] = 0;


*/