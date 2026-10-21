#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO "/tmp/myfifo"
#define SIZE 256
int main(int argc, char *argv[]) {

    mkfifo("/fifo",0666);

    char message[SIZE];

    if (strcmp(argv[1], "writer") == 0) {
        int fd = open(FIFO, O_WRONLY);

        while (1) {

            printf("Writer: ");
            fflush(stdout);

            if (fgets(message, SIZE, stdin) == NULL)
                strcpy(message, "exit\n");

            write(fd, message, strlen(message));

            if (strncmp(message, "exit\n", 5) == 0)break;

        }

           close(fd);
    }

    else{

        int fd = open(FIFO, O_RDONLY);

        FILE *stream = fdopen(fd, "r");

        while (fgets(message, SIZE, stream) != NULL) {

            message[strcspn(message, "\n")] = '\0';

            if (strcmp(message, "exit") == 0)break;

            printf("Reader received: %s\n", message);
        }

        fclose(stream);


    }

}