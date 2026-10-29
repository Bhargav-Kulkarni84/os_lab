#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){

    pid_t result = fork();
    char msg1 [] = "I am parent\n"; 
    char msg2 [] = "I am child\n"; 

    char orphanmsg [] = "Parent is terminating before child\n";
    char childTermination [] = "Child is terminating\n";

    if(result>0){
        write(1,msg1,sizeof(msg1));
        write(1,orphanmsg,sizeof(orphanmsg));
    }

    if(result == 0){
        write(1,msg2,sizeof(msg2));
        sleep(5);
        write(1,childTermination,sizeof(childTermination));
    }


    return 0;
}