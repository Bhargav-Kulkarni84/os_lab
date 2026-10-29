#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char* argv[]){

    bool isWriter = strcmp("writer",argv[1]) == 0;

    if(isWriter){

        //create a pipe
        mkfifo("/tmp/fifo",0600);
    
        //Create write only file descriptor.
        int fd = open("/tmp/fifo",O_WRONLY);
        
        while(true){

            char buffer[100];
            printf("Enter a message to be sent\n");
            scanf("%99s",buffer);
            
            write(fd,buffer,strlen(buffer)+1);

            if(strcmp(buffer,"exit")==0) break;

        }

    }

    else{

        //create a pipe
        mkfifo("/tmp/fifo",0600);
    
        //Create read only file descriptor.
        int fd = open("/tmp/fifo",O_RDONLY);

        char buffer[100];
        int index = 0;
        
        while(true){
            
            char c;
            int n = read(fd,&c,1);
            
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

        close(fd);
        
    }

}