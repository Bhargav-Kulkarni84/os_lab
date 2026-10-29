#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define N 5

enum {THINKING, EATING, HUNGRY};

sem_t sem [N];
pthread_t thread[N];
int state[N];
sem_t mutex;

void test(int id){

    if(state[id] == HUNGRY && state[(id+4)%N] != EATING && state[(id+1)%N] != EATING){
        state[id] = EATING;
        sem_post(&sem[id]);
        printf("%d philosopher is Eating\n",id);
    }
    
}

void pick(int id){

    sem_wait(&mutex);
        state[id] = HUNGRY;
        printf("%d philosopher is Hungry\n",id);
        test(id);
    sem_post(&mutex);

    sem_wait(&sem[id]);

}

void drop(int id){

    sem_wait(&mutex);

        state[id] = THINKING;

        int left = (id+4)%N;
        int right = (id+1)%N;

        test(left);
        test(right);

    sem_post(&mutex);

}

void* process(void* args){

    int id = *((int*)args);

    for(int i=0; i<5; i++){

        // Thinking 
        printf("%d philosopher is thinking\n",id);
        sleep(2);
        
        //Pick fork
        pick(id);

        //Eat
        sleep(3);

        //Release fork
        drop(id);
        
    }

    return NULL;

}

int main(){

    int ids [N] = {0,1,2,3,4,};

    for(int i=0; i<N; i++){
        state[i] = THINKING;
    }

    for(int i=0; i<N; i++){
        sem_init(&sem[i],0,0);
    }

    sem_init(&mutex,0,1);

    for(int i=0; i<N; i++){
        pthread_create(&thread[i],NULL,process,&ids[i]);
    }

    for(int i=0; i<N; i++){
        pthread_join(thread[i],NULL);
    }

    return 0;
}