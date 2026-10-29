// Hit ratio
// No of page faults
// % Page fault

#include<stdio.h>
#include<stdbool.h>

bool contains(int frameSize, int frames[], int target){

    for(int i=0; i<frameSize; i++){
        if(frames[i] == target) return true;
    }

    return false;

}

void fifo(int n, int seq[],int frameSize){
 
    int hits = 0;
    int faults = 0;
    int frames [frameSize];

    int idx = 0;

    for(int i=0; i<n; i++){

        int target = seq[i];

        if(contains(frameSize,frames,target)) hits++;

        else{
            faults++;
            frames[idx] = target;
            idx = (idx+1)%frameSize;
        }

    }
    
    double percentFaults = ((double)faults/n)*100;
    double hitRatio = ((double) hits/n)*100;

    printf("FIFO\n");
    printf("Total Page Hits = %d \n",hits);
    printf("Total Page Faults = %d\n",faults);
    printf("% Page Faults = %0.2f\n", percentFaults);
    printf("% Hit Ratio = %0.2f\n", hitRatio);
    printf("____________________________________________________\n");


}

int replaceIndex(int startIdx, int n, int seq[], int frames[],int frameSize){

    int indices[frameSize];

    for(int i=startIdx; i<n; i++){

        int currframe = frames[startIdx];
        
        bool found = false;
        for(int j=0; j<frameSize; j++){
            
            if(currframe == frames[j]){
                found = false;
                indices[j] = min(indices[j],i);
            } 
        }
        if(!found) return i;
    }

    int minValIdx = (frameSize+1)%frameSize;

    for(int i=0; i<frameSize; i++){

        if(minValIdx>indices[i]){
            minValIdx = i;
        }
    }

    return minValIdx;

}



void optimal(int n, int seq[],int frames){

}

void lru(int n, int seq[],int frames){

}

int main(){

    int n  = 20;
    int seq [20] = {1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6};
    int frames = 4;

    fifo(n,seq,frames);
    optimal(n,seq,frames);
    lru(n,seq,frames);

}