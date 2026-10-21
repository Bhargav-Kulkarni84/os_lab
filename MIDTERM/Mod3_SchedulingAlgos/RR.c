#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include<math.h>

//Define the time quantum.
#define tq 2

int count = 0;
int front = 0;
int back = 0;

typedef struct Process{
    
    int pid;
    int rem_bt;
    int inqueue;

    int at;
    int bt;
    int ct;
    int tat;
    int wt;


} Process; 

//Sort according to pid
void sort_at(int n, Process processes[n]){

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            int curr = processes[j].at;
            int next = processes[j+1].at;

            if(next<curr){
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }

        }
    }

}

//Sort according to pid
void sort_pid(int n, Process processes[n]){

    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){

            int curr = processes[j].pid;
            int next = processes[j+1].pid;

            if(next<curr){
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }

        }
    }

}

//Mark processes as completed
bool completed(int n, Process processes[n]){

    for(int i=0; i<n; i++){
        if(processes[i].rem_bt > 0) return false;
    }

    return true;

}

//Add the process at the back in the queue when it has arrived.
void add(int n, int currentTime, Process processes[n], Process* q[n]){

    for(int i=0; i<n; i++){

        Process* p = &processes[i];

        if(p->inqueue == true) continue;

        if(p->at <= currentTime){
            p->inqueue = true;
            q[back] = p;
            count++;
            back = (back+1) % n;
        }

    }

}

int main(){

    /*
    int n;
    char message[] = "Enter no of processes";
    write(1,message,sizeof(message)-1);
    read(0,n,sizeof(n));

    Process processes[n];

    for(int i=0; i<n; i++){

        Process* p = &processes[i];

            for(int i=0; i<n; i++){

        printf("Enter the Arrival Time(at)\n");
        scanf("%d",&processes[i].at);

        printf("Enter the CPU Burst Time(bt)\n");
        scanf("%d",&processes[i].bt);

    }
    */

    int n = 6;
    Process processes [n];
    
    int at[] = {0,1,2,3,4,6};
    int bt[] = {4,5,2,1,6,3};
    
    for(int i=0; i<n; i++){
        processes[i].pid = i+1;
        processes[i].at = at[i];
        processes[i].bt = bt[i];
        processes[i].rem_bt = processes[i].bt;
        processes[i].ct = 0;
        processes[i].tat = 0;
        processes[i].wt = 0;
        processes[i].inqueue = false;
    }

    //Queue to keep track of all the existing processes.
    Process *q[n];
    int currTime = 0;

    //Sort the processes according to its arrival time.
    sort_at(n,processes);
    
    //Increment current time till we have atleast once process inside queue.
    while(count == 0){
        add(n,currTime,processes,q);
        currTime += 1;
    }

    while(!completed(n,processes)){
        
        //If no process has arrived or is inside the queue.
        //Increment the current time by 1 and check for available processes.
        if(count == 0){
            currTime++;
            add(n,currTime,processes,q);
            continue;
        }

        Process* curr = q[front];
        int rt = tq;
        if(curr->rem_bt < tq) rt = curr->rem_bt;
        
        //If there is any process inside the queue.
        //Execute it for the processe's required time.
        //Add the process that has arrived in the mean time to the queue.
        for(int i=0; i<rt; i++){
            //Execute process for time quantum / required time.
            curr->rem_bt -= 1;
            add(n,currTime,processes,q);
            currTime+=1;     
        }
        
        //If bt = 0 --> Process completed
        if(curr->rem_bt == 0){  
            curr->ct = currTime-1;
            curr->tat = curr->ct - curr->at;
            curr->wt = curr->tat - curr->bt;
        }

        //Remove the current process of off queue
        count-=1;

        //Move to the next index.
        front = (front+1)%n;

        if(curr->rem_bt > 0){
            q[back] = curr;
            back = (back+1)%n;
            count++;
        }

    }

    //Print info
    float avgTAT = 0;
    float avgWT = 0;
    for(int i=0; i<n; i++){
        Process* p = &processes[i];            
        printf("%d\t %d\t %d\t %d\t %d\t \n",p->at,p->bt,p->ct,p->tat,p->wt);
        avgTAT+=p->tat;
        avgWT+=p->wt;
    }

    printf("Averge TAT = %0.2f\n",avgTAT/n);
    printf("Averge WT = %0.2f\n",avgWT/n);
        
        
}
