#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define WRITE_END 1
#define READ_END 0


char message[17] = "This Is A Message";

void check_err(int statusCode, char *errMsg);
void parent(int pipefd1[2], int pipefd2[2]);
void child(int pipefd1[2], int pipefd2[2]);
void convertStr(char in[]);

int main(){
    int pipefd1[2];
    int pipefd2[2];

    pid_t pid;

    check_err(pipe(pipefd1), "pipe1");
    check_err(pipe(pipefd2), "pipe2");

    pid = fork();
    check_err(pid, "fork");

    if (pid != 0) {  
        parent(pipefd1, pipefd2);

    } else {
        child(pipefd1, pipefd2);
    }
    return 0;
}
void check_err(int statusCode, char *errMsg){
    if(statusCode == -1){   
        perror(errMsg);
        exit(EXIT_FAILURE);
    }
}
void parent(int pipefd1[2], int pipefd2[]){
    close(pipefd1[READ_END]);
    close(pipefd2[WRITE_END]);

    printf("[BEFORE]%s\n", message);
    write(pipefd1[WRITE_END], message, sizeof(message));        
    close(pipefd1[WRITE_END]);   

    read(pipefd2[READ_END], message, sizeof(message));
    close(pipefd2[READ_END]);
    printf("[AFTER]%s\n", message);
}
void child(int pipefd1[2], int pipefd2[2]){
    
    close(pipefd1[WRITE_END]);
    close(pipefd2[READ_END]);

    char inputMessage[17];
    read(pipefd1[READ_END], inputMessage, sizeof(inputMessage));
    close(pipefd1[READ_END]);

    convertStr(inputMessage);
    write(pipefd2[WRITE_END], inputMessage, sizeof(inputMessage));
    close(pipefd2[WRITE_END]);   
}
void convertStr(char in[17]){

    for(int i=0; i<17 ; i++){
        if(isupper(in[i]))
            in[i] = tolower(in[i]);
        else
            in[i] = toupper(in[i]);
    }

}