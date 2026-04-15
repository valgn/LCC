#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

// Función simplificada y SEGURA para la memoria
char **get_args(char *buffer, int *cantidad)
{
    // Reservamos espacio para hasta 50 argumentos (suficiente para una mini shell)
    char **args = malloc(sizeof(char *) * 50); 
    int idx = 0;

    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        // strlen + 1 es VITAL para guardar el '\0' y no romper la memoria
        args[idx] = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(args[idx], token);
        token = strtok(NULL, " ");
        idx++;
    }
    args[idx] = NULL;
    *cantidad = idx; 
    return args;
}

int main()
{
    char buffer[100];
    char **com;
    int cantidad;
    
    while (1)
    {
        printf(">>> ");
        if (fgets(buffer, 100, stdin) == NULL) break; // Por si aprietan Ctrl+D
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "quit") == 0) break;
        
        com = get_args(buffer, &cantidad);


        int num_args = 0;
        while (com[num_args] != NULL) {
            num_args++;
        }

        int cantidad_comandos = 1;
        for (int i = 0; i < num_args; i++)
        {
            if (com[i] != NULL && strcmp(com[i], "|") == 0)
            {
                cantidad_comandos++;
            }
        }
        
    // 3. Reservar la memoria justa para los punteros de los comandos
        char ***comandos = malloc(sizeof(char **) * (cantidad_comandos + 1));
        int indice = 0;
        comandos[indice] = &com[0];
        indice++;

        // 4. Reemplazar los '|' por NULL, de forma 100% segura
        for (int i = 0; i < num_args; i++)
        {
            if (com[i] != NULL && strcmp(com[i], "|") == 0)
            {
                com[i] = NULL; // Cortamos el string para execvp
                comandos[indice] = &com[i + 1]; // Apuntamos al siguiente comando
                indice++;
            }
        }
        comandos[indice] = NULL;
    
        for(int i = 0; i < indice; i++){
            for(int j = 0; j < num_args; j++){
                printf("%s ", comandos[i][j]);
            }
        }
    
    }


    return 0;
}