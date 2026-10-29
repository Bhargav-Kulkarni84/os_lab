#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>

int main(){

    //Create write only file descriptor.
    int fd [2];
    pipe(fd);

    pid_t pid = fork();

    if(pid>0){
        
        //close read
        close(fd[0]);

        while(true){

            char buffer[100];
            printf("Enter a message to be sent :");
            scanf("%99s",buffer);
            
            write(fd[1],buffer,strlen(buffer)+1);

            if(strcmp(buffer,"exit")==0) break;

        }

        close(fd[1]);

    }

    else{

        char buffer[100];
        int index = 0;

        // Close write
        close(fd[1]);
        
        while(true){
            
            char c;
            int n = read(fd[0],&c,1);
            
            if(c == '\0'){
                buffer[index] = '\0';
                printf("Reader reads = %s\n",buffer);
                index = 0;

                if(strcmp(buffer,"exit")==0) break;
            }
            else if(index < sizeof(buffer) - 1){
                buffer[index++] = c;
            }

        }
        
        // Close read
        close(fd[0]);
        
    }

}