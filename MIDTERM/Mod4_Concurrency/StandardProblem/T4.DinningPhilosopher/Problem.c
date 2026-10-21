#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define N 5
#define THINKING 0
#define EATING 1
#define HUNGRY 2

int state [N];
int id [N];

sem_t mutex;
sem_t self[N];

void test(int i){

    if(state[i] == HUNGRY && state[(i-1+N)%N] != EATING && state[(i+1+N)%N] != EATING){
        state[i] = EATING;
        sem_post(&self[i]);
    }

}

void pick_fork(int i){

    sem_wait(&mutex);
        state[i] = HUNGRY;
        printf("Philosopher %d is hungry\n",i);
        test(i);
    sem_post(&mutex);

    sem_wait(&self[i]);

}

void release_fork(int i){

    sem_wait(&mutex);
        state[i] = THINKING;
        printf("Philosopher %d is THINKING\n", i);
        test((i-1+N)%N);
        test((i+1+N)%N);
    sem_post(&mutex);

}

void* philosopher(void* args){
    
    int i = *((int*)args);

    for (int r = 0; r<3; r++) {
        sleep(1);                    // think
        pick_fork(i);
        printf("Philosopher %d is eating\n", i);
        sleep(2);                    // eat
        release_fork(i);
    }
    return NULL;
}

int main(){

    pthread_t thread[N];

    sem_init(&mutex,0,1);

    for(int i=0; i<N; i++){
        sem_init(&self[i],0,0);
        state[i] = THINKING;
        id[i] = i;
    }

    for(int i=0; i<N; i++){
        pthread_create(&thread[i],NULL,philosopher,&id[i]);
    }

    for(int i=0; i<N; i++){
        pthread_join(thread[i],NULL);
    }

    return 0;


}