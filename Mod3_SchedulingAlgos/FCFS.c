/*

    AT BT CT TAT WT 

    User input
    AT,BT,CT

    Programatically Computed
    TAT = CT - AT
    WT = TAT - BT

*/

#include<stdio.h>

typedef struct{
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
} Info;

void sort(int n, Info processes[n]){

    //Sort according to Arrival Time

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            int curr = processes[j].at;
            int next = processes[j+1].at;

            if(next<curr){
                Info temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }

        }
    }

}

int main(){

    int n;

    printf("Enter no of processes\n");
    scanf("%d",&n);

    Info processes [n];

    for(int i=0; i<n; i++){

        printf("Enter the Arrival Time(at)\n");
        scanf("%d",&processes[i].at);

        printf("Enter the CPU Burst Time(bt)\n");
        scanf("%d",&processes[i].bt);

    }

    //Compute the CT,TAT, WT
    printf("AT BT CT TAT WT\n");

    int startTime = 0;
    for(int i=0; i<n; i++){
        
        Info *p = &processes[i];
        
        //Execute each process till its completions
        //Meaning the ct = bt - at for each processs.

        p->ct = startTime + p->bt;
        p->tat = p->ct - p->at;
        p->wt = p->tat - p->bt;

        printf("%d, %d, %d, %d, %d \n",p->at,p->bt,p->ct,p->tat,p->wt);

        startTime+= p->bt;

    }



}