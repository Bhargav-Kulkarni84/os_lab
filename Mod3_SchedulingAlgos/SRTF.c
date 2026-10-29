// Premptive Shortest job first

#include<stdio.h>
#include<stdbool.h>

void sort_pid(int n, Process processes []){

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            if(processes[j].pid > processes[j+1].pid){
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }
}

void sort_bt(int n, Process processes []){

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            if(processes[j].rem_bt > processes[j+1].rem_bt){
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }

            else if(processes[j].bt > processes[j+1].bt){
                if(processes[j].at > processes[j+1].at){
                    Process temp = processes[j];
                    processes[j] = processes[j+1];
                    processes[j+1] = temp;
                }   
            }

        }
    }

}

bool completed(int n, Process processes []){
    for(int i=0; i<n; i++) if(processes[i].rem_bt > 0) return false;
    return true;
}

int getIndex(int n, Process processes[], int currTime){

    for(int i=0; i<n; i++){
        Process* p = &processes[i];
        if(p->at<=currTime && p->rem_bt>0) return i;
    }

    return -1;

}

typedef struct Process{

    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rem_bt;

} Process;

int main(){

    int at[] = {0,0,0,0};
    int bt[] = {7,4,2,5};
    int n = 4;

    Process processes[n];

    for(int i=0; i<n; i++){

        Process* p = &processes[i];
        p->pid = i+1;
        p->at = at[i];
        p->bt = bt[i];
        p->rem_bt = bt[i];

    }

    sort_bt(n,processes);

    int currTime = 0;

    while(!completed(n,processes)){

        int idx = getIndex(n,processes,currTime);

        if(idx == -1){
            currTime++;
            continue;
        }

        Process* p = &processes[idx];

        if(p->at<=currTime && p->rem_bt>0){

            p->rem_bt = p->rem_bt - 1;
            if(p->rem_bt == 0){
                p->ct = currTime - p->at;
                p->tat = p->ct - p->at;
                p->wt = p->tat - p->bt;
            }
        
        }
        
        currTime++;
        sort_bt(n,processes);

    }

    sort_pid(n,processes);

    for(int i=0; i<n; i++){
        printf("+---");
    }

    printf("+\n");

    for(int i=0; i<n; i++){
        printf("| P%d",i+1);
    }

    for(int i=0; i<n; i++){
        printf("+---");
    }

    for(int i=0; i<n; i++){
        printf("%4d",processes->bt);
    }

}