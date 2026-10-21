/*

    AT BT CT TAT WT 

    User input
    AT,BT,CT

    Programatically Computed
    TAT = CT - AT
    WT = TAT - BT

*/

#include<stdio.h>
#include<unistd.h>
#include<limits.h>

#define threshold 10

typedef struct{
    int pid;
    int priority;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rem_bt;
} Info;

//Sort according to priority
void sort_priority(int n, Info processes[n]){

    //Sort according to priority Time

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            int curr = processes[j].priority;
            int next = processes[j+1].priority;

            if(next<curr){
                Info temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
            else if(next == curr){
                if(processes[j].at > processes[j+1].at){
                    Info temp = processes[j];
                    processes[j] = processes[j+1];
                    processes[j+1] = temp;
                }
            }
        }
    }

}

int getLowestPrioriy(int n, int currTime, Info processes[n]){

    int highestPriority = INT_MAX;
    int highestPriorityIdx = -1;

    for(int i=0; i<n; i++){

        Info* p = &processes[i];

        if(p->rem_bt>0 && p->at <= currTime){

            if(p->priority < highestPriority){
                highestPriority = p->priority;
                highestPriorityIdx = i;
            }
            else if(p->priority == highestPriority){
                if(p->at < processes[highestPriorityIdx].at){
                    highestPriority = p->priority;
                    highestPriorityIdx = i;
                }
            }

        }
    }

    return highestPriorityIdx;

}

bool allProcessesExecuted(int n, Info processes[n]){

    for(int i=0; i<n; i++){
        Info* p = &processes[i];
        if(p->rem_bt > 0) return false;;
    }

    return true;

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
    
    // int n = 4;
    // int at[] = {0,1,2,4};
    // int bt[] = {5,4,2,1};
    // int priority[] = {2,1,3,1};

    int n = 3;
    int at[] = {0,1,2};
    int bt[] = {12,3,4};
    int priority[] = {2,1,3};
    
    Info processes [n];

    for(int i=0; i<n; i++){
        processes[i].pid = i+1;
        processes[i].priority = priority[i];
        processes[i].at = at[i];
        processes[i].bt = bt[i];
        processes[i].rem_bt = processes[i].bt;
        processes[i].ct = 0;
        processes[i].tat = 0;
        processes[i].wt = 0;
    }

    //Sort the processes according to priority;
    sort_priority(n,processes);

    int currTime = 0;

    //Check if rem_bt of each process in 0
    while(!allProcessesExecuted(n,processes)){

        //Get the lowest priority process.
        int processIdx = getLowestPrioriy(n,currTime,processes);
        Info* process = &processes[processIdx];
        
        //Run it for 1 time interval.
        process->rem_bt -= 1;
        currTime+=1;

        //Check if process completed its execution. 
        //If so compute tat and wt.
        if(process->rem_bt == 0){
            process->ct = currTime;
            process->tat = process->ct - process->at;
            process->wt = process->tat - process->bt;
        }

    }

    sort_pid(n,processes);
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


    for(int i=0; i<n; i++){
        Info *p = &processes[i];            
        if(p->wt > threshold){
            printf("Process-%d has starvation as wt=%d > %d \n",p->pid,p->wt,threshold);
        }
    }
        
    return 0;

}