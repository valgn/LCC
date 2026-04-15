#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


void handler(int signum){
    printf("Señal recibida.\n");
}

int main(){
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    pid_t pid = fork();
    if(pid == 0){
        sigaction(SIGUSR1,&sa, NULL);
        pause();
        exit(0);
    }
    else{
        sleep(1);
        kill(pid, SIGUSR1);
        wait(0);
    }
    return 0;
}