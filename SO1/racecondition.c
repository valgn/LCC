#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
    Sincronizacion se produce cuando uno o mas procesos depende del comportomaniento de otro porceoso. Hay 
    competencia o cooperacion. 

    Competencia: Varios proceso compiten para ejectura una instruccion. 
    Ejemplo: Dos procesos intneta escribir un string

*/
void characterAtTime(char* str){
    setbuf(stdout, NULL); //no bufferea
    char *ptr;
    int c;
    for(ptr = str; (c = *ptr++)!= 0;){
        putc(c, stdout);
    }

}
int main(){
    pid_t pid = fork();
    // int fdes[2];
    // pipe(fdes);
    char c;


    if (pid == 0){
        characterAtTime("output from child\n");

    }
    else{
        characterAtTime("Output from parent\n");
        
    }

    

    return 0;
}