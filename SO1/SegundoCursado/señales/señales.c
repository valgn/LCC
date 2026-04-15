#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void handler(int s){
    printf("Señal recibida.\n");
}

int main(){
    pid_t pid = fork();
    if(pid == 0){
        signal(SIGUSR1,handler);
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