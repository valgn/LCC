#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

//execl("/bin", cm, ... ... NULL)

char** get_args(char *buffer, int* cantidad) {
    int cant = 0;
    int max_len = 0;

   
    int actual_len = 0;
    for(int i = 0; buffer[i] != '\0'; i++){
        if (buffer[i] == ' '){
            cant += 1;
            if (actual_len > max_len) max_len = actual_len;
            actual_len = 0;
        }
        actual_len += 1;
    }
    cant += 2;

    (*cantidad) = cant;
    char **args = malloc(sizeof(char*)*cant);

    char *token = strtok(buffer, " "); 
    
    int idx = 0;
    while (token != NULL) {
        args[idx] = malloc(sizeof(char)*max_len);
        strcpy(args[idx], token);
        token = strtok(NULL, " ");
        idx++;
    }
    args[idx] = NULL;
    return args;
}

int main(){
    char buffer[100];
    char** com;
    int status;
    int cantidad;
    while(1){
        printf(">>> ");
        status = 0;
        fgets(buffer, 100, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if(strcmp(buffer, "quit") == 0) break;
        com = get_args(buffer, &cantidad);
        
        int haypipe = 0;
        int pipefd[2];
        char** com1 = com;
        char** com2 = NULL;
        
        for(int i = 0; com[i] != NULL; i++){ //FIJARSE SI HAY PIPES
            if(strcmp(com[i], "|") == 0){
                haypipe = 1;
                pipe(pipefd);
                com[i] = NULL;
                com2 = &com[i+1];
                break;
            }
        }
        if(haypipe){
            pid_t hijo1 = fork();
            if(hijo1 == 0){
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                execvp(com1[0], com1);
                perror("Error"); exit(EXIT_FAILURE);
            }
            pid_t hijo2 = fork();
            if(hijo2 == 0){
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                execvp(com2[0], com2);
                perror("Error"); exit(EXIT_FAILURE);
            }
            close(pipefd[0]); close(pipefd[1]);
            wait(0); wait(0);
        }


        else{
            pid_t pid = fork();

            if(pid == 0){ //HIJO
                for(int i = 0; com[i] != NULL; i++){
                    if(strcmp(com[i], ">") == 0){
                        char* archivo = com[i + 1];
                        int fd = open(archivo, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        com[i] = NULL;
                    }
                }
                status = execvp(com[0], com); //execvp hace exit
                perror("Error"); 
                exit(EXIT_FAILURE);
            }

            else{ //PADRE
                wait(0);
                for(int i = 0; i < cantidad; i++) free(com[i]);
                free(com);
            }
        }

    printf("\n");
    }

    return 0;
}