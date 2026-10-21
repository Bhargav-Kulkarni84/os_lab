#include<stdio.h>
#include<stdbool.h>

bool valid(int p, int resources, int currentAvailable [resources], int need [][resources]){

    for(int i=0; i<resources; i++){
        if(currentAvailable[i] < need[p][i]) return false;
    }

    return true;

}

void updateAvailable(int p, int resources, int currentAvailable[resources], int allocation[][resources]){

    for(int i=0; i<resources; i++){
        currentAvailable[i] += allocation[p][i];
    }

}

bool allCompleted(int processes, bool completed[]){
    
    for(int i=0; i<processes; i++){
        if(!completed[i]) return false;
    }

    return true;
}

int main(){
    
    //Get the no of processes and avaialble resources.
    int processes = 5;
    int resources = 3;

    //Get the instances of resouces, allocation and max matrix
    int totalAvailable[3] = {10,5,7};
    int allocation[5][3] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    int max [5][3] = {{7,5,3},{3,2,2},{9,0,2},{4,2,2},{5,3,3}};

    //Declare available and need matrix
    int available[processes][resources];
    int need[processes][resources];

    //Compute total allocation of resouces, inorder to compute availability matrix (totalAvailable - totalAllocation)
    int totalAllocation [resources];
    for(int i=0; i<processes; i++){
        for(int j=0; j<resources; j++){
            totalAllocation[j] += allocation[i][j];
        }
    }

    //Compute need matrix
    for(int i=0; i<processes; i++){
        for(int j=0; j<resources; j++){
            need[i][j] = max[i][j]-allocation[i][j];
        }
    }

    //Find the current available resouces from process-0
    int currentAvailable[resources];
    for(int i=0; i<resources; i++){
        currentAvailable[i] = totalAvailable[i] - totalAllocation[i];
    }

    //For current available process check every need matrix row, 
    //If current available resource can fullfill need, add the allocation resouces to the current avaialable resouces
    //Other wise check for other processes 

    //completed array to keep track of all the processes completed so far
    bool completed [processes];

    //Safe state matrix for storing the safe state. 
    int index = 0;
    int safeState[processes];
    
    //Iterate while we are able to satisfy atleast one process.
    bool safeStateExists = true;
    while(true){
        
        //Variable to keep track, if any process changed.
        bool processing = false;

        for(int p=0; p<processes; p++){
    
            if(!completed[p]){

                bool canComplete = valid(p,resources,currentAvailable,need);

                //Update the total available.
                if(canComplete){
                    updateAvailable(p,resources,currentAvailable,allocation);
                    processing = true;
                    completed[p] = true;
                    safeState[index++] = p;
                } 

            }
    
        }

        if(allCompleted(processes,completed)) break;

        if(!processing){
            printf("No safe state exists\n");
            safeStateExists = false;
            break;
        } 

    }

    if(safeStateExists){
        printf("Safe State\n"); 
        for(int i=0; i<processes; i++){
            printf("%d-->",safeState[i]);
        }
    }


}