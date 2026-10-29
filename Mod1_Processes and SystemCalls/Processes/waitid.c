#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include <sys/wait.h>

int main(){

    pid_t rval = fork();
    int childID;

    if(rval<0){
        perror("Error creating fork");
    }
    
    if(rval > 0){
        
        char parentMsg [] = "I am parent process\n";
        write(1,parentMsg,sizeof(parentMsg)-1);

        char parentWaiting[100];
        
        snprintf(parentWaiting,sizeof(parentWaiting),"My job is completed, waiting for all of my childs to complete its execution\n");
        write(1,parentWaiting,sizeof(parentWaiting)-1);
        
        wait(&childID);

        if(WIFEXITED(childID)){

            char parentMsg2[100];
            snprintf(parentMsg2,sizeof(parentMsg2),"Child Completed execution, termination parent with pid %d\n", WEXITSTATUS(childID));
            write(1,parentMsg2,sizeof(parentMsg2)-1);

        }

    }
    
    else{
        char childMsg1 [] = "I am child process, it will take me 5 secs to finish\n";
        write(1,childMsg1,sizeof(childMsg1)-1);

        sleep(5);

        char childMsg2 [] = "Child execution finished\n";
        write(1,childMsg2,sizeof(childMsg2)-1);

        return getpid();

    }

}