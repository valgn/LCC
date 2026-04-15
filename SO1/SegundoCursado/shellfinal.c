#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

// execl("/bin", cm, ... ... NULL)

char **get_args(char *buffer, int *cantidad)
{
    int cant = 0;
    int max_len = 0;
    int actual_len = 0;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == ' ')
        {
            cant += 1;
            if (actual_len > max_len)
                max_len = actual_len;
                actual_len = 0;
        }
        actual_len += 1;
    }
    cant += 2;
    (*cantidad) = cant;
    char **args = malloc(sizeof(char *) * cant);
    char *token = strtok(buffer, " ");

    int idx = 0;
    while (token != NULL)
    {
        args[idx] = malloc(sizeof(char) * max_len);
        strcpy(args[idx], token);
        token = strtok(NULL, " ");
        idx++;
    }
    args[idx] = NULL;
    return args;
}


int main()
{
    char buffer[100];
    char **com;
    int status;
    int cantidad;
    while (1)
    {

        printf(">>> ");
        status = 0;
        fgets(buffer, 100, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "quit") == 0)
            break;

        com = get_args(buffer, &cantidad);

        int cantidad_comandos = 1;

        for (int i = 0; com[i] != NULL; i++)
        {
            if (strcmp(com[i], "|") == 0)
            {
                cantidad_comandos++;
            }
        }

        char ***comandos = malloc(sizeof(char **) * (cantidad_comandos + 1));
        int indice = 0;
        comandos[indice] = &com[0];
        indice++;

        for (int i = 0; com[i] != NULL; i++)
        {
            if (strcmp(com[i], "|") == 0)
            {
                com[i] = NULL;
                comandos[indice] = &com[i + 1];
                indice++;
            }
        }
        comandos[indice] = NULL;

        int pipefd[2];
        int in_fd = 0; // File descriptor que engancha todo

        for (int i = 0; i < cantidad_comandos; i++)
        {
            if (i < cantidad_comandos - 1)
            {
                pipe(pipefd);
            }

            pid_t pid = fork();

            if (pid == 0)
            {
                if (in_fd != 0)
                {
                    dup2(in_fd, STDIN_FILENO);
                    close(in_fd);
                }

                if (i < cantidad_comandos - 1)
                {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                    close(pipefd[0]);

                }
                for(int j = 0; comandos[i][j] != NULL; j++){
                    if(strcmp(comandos[i][j], ">") == 0){
                        char* archivo = comandos[i][j+1];
                        int file_des = open(archivo, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                        dup2(file_des, STDOUT_FILENO); 
                        close(file_des);
                        comandos[i][j] = NULL; // Para poder ejecutar en la salida
                    }
                }
                execvp(comandos[i][0], comandos[i]);
                perror("Error");
                exit(EXIT_FAILURE);

            }
            else{
                if(in_fd != 0) close(in_fd);
                if(i < cantidad_comandos - 1){
                    close(pipefd[1]);
                    in_fd = pipefd[0];

                }

            }
        }
        for(int i = 0; i < cantidad_comandos; i++){
            wait(NULL);
        }

        printf("\n");
    }
    return 0;
}