#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    int pipefd[2];
    pid_t pid;
    char buff[] = "women,cant live with them";
    
    if(pipe(pipefd) == -1){
        perror("Error");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    if(pid == 0){
        close(pipefd[1]);
        while(read(pipefd[0], &buff, 1) > 0){
            write(STDOUT_FILENO, &buff, 1);
        }
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }   
    else{
        close(pipefd[0]);
        write(pipefd[1], buff, sizeof(buff));
        close(pipefd[1]);
        wait(0);
        exit(EXIT_SUCCESS);
    } 


    return 0;
}