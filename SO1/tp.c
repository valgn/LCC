#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define MAX 128


/*
versionado de codigo: seguimietnos de cambio, colaboracion, resolucion de rrores, mantenimiento del proyecto. 
delta-based se van guardando los cambios
snapshot: se guarda com ouna foto comṕleta de todo el sistema.

git guarda clave valor, se guarda haciendo un commit. se puede ramificar y fusionar

git init. Se crea un repositorio de tipo git vacio



*/

int main() {
    int status;
    char buff[MAX], *token, *tokensArr[MAX];
    char path[MAX + 5] = "/bin/";
    const char EXITCMD[] = "exit";

    while(1) {
        
        printf("> ");
        fgets(buff,MAX,stdin);
        buff[strlen(buff)-1] = '\0';
        
        if(strcmp(buff,EXITCMD) == 0) 
            return 0;
        
        __pid_t pid = fork();
        if (pid == 0) {
            if (strlen(buff) == 0)
                exit(0);
            
            int i = 0;
            for(token = strtok(buff," "); token != NULL; i++) {
                tokensArr[i] = token;
                

                if(strcmp(token,">") == 0){
                    token= strtok(NULL, " ");
                    FILE* f = fopen(token, "w");
                    int fd = fileno(f);
                    dup2(fd,STDOUT_FILENO);
                    token= strtok(NULL, " ");
                }
                else{
                    token = strtok(NULL," ");
                }

            }
            tokensArr[i] = NULL;

            
            
            strcat(path,tokensArr[0]);
            int error = execv(path, tokensArr);
            
            if (error == -1)
                exit(0);
        }
        else {
            wait(&status);
        }
    }
    return 0;
}