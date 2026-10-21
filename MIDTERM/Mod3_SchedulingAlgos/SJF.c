/*

    SJF : Shortest Job first.
    Sort the processes according to bt.
    Pick the processes for which at<=currTime
    No preemption so execute it completely.

    AT BT CT TAT WT 

    User input
    AT,BT,CT

    Programatically Computed
    TAT = CT - AT
    WT = TAT - BT

*/

#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>

typedef struct{
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rem_bt;
} Info;

//Sort according to burst time
void sort_bt(int n, Info processes[n]){

    //Sort according to Arrival Time

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            int curr = processes[j].bt;
            int next = processes[j+1].bt;

            if(next<curr){
                Info temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }

        }
    }

}

//Sort according to pid
void sort_pid(int n, Info processes[n]){

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            int curr = processes[j].pid;
            int next = processes[j+1].pid;

            if(next<curr){
                Info temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }

        }
    }

}

//Check if all processses completed their execution (bt = 0)
bool allProcessesExecuted(int n, Info processes[n]){

    for(int i=0; i<n; i++){
        Info* p = &processes[i];
        if(p->rem_bt>0) return false;;
    }

    return true;

}

int main(){

/*
    int n;

    printf("Enter no of processes\n");
    scanf("%d",&n);

    Info processes [n];

    for(int i=0; i<n; i++){

        processes[i].pid = i+1;

        printf("Enter the Arrival Time(at)\n");
        scanf("%d",&processes[i].at);

        printf("Enter the CPU Burst Time(bt)\n");
        scanf("%d",&processes[i].bt);

        processes[i].rem_bt = processes[i].bt;
        processes[i].ct = 0;
        processes[i].tat = 0;
        processes[i].wt = 0;
    }

*/
    int at[] = {0,0,0,0};
    int bt[] = {7,4,2,5};
    int n = 4;
    Info processes [n];
    for(int i=0; i<n; i++){

        processes[i].pid = i+1;
        processes[i].at = at[i];
        processes[i].bt = bt[i];
        processes[i].rem_bt = processes[i].bt;
        processes[i].ct = 0;
        processes[i].tat = 0;
        processes[i].wt = 0;
    }

    //Sort the processes according to bt;
    sort_bt(n,processes);

    int currTime = 0;

    //Check if rem bt of each process in 0
    while(!allProcessesExecuted(n,processes)){
        
        for(int i=0; i<n; i++){
            
            Info *p = &processes[i];

            //If process arrival time is greater than current time, don't give a process chance.
            if(p->at > currTime) continue;
            
            //if process hasn't completed yet. 
            if(p->rem_bt > 0){
                p->ct = p->bt + currTime;
                p->rem_bt = 0;
                p->tat = p->ct - p->at;
                p->wt = p->tat - p->bt;
                currTime+=(*p).bt;
            }
            
        }
        
    }

    sort_pid(n,processes);
    //Print the CT, TAT, WT
    printf("AT\t BT\t CT\t TAT\t WT\n");
    
    float avgTAT = 0;
    float avgWT = 0;
    for(int i=0; i<n; i++){
        Info *p = &processes[i];            
        printf("%d\t %d\t %d\t %d\t %d\t \n",p->at,p->bt,p->ct,p->tat,p->wt);
        avgTAT+=p->tat;
        avgWT+=p->wt;
    }

    printf("Averge TAT = %0.2f\n",avgTAT/n);
    printf("Averge WT = %0.2f\n",avgWT/n);
        
}