#include <string.h>
#include <stdio.h>

char *busca(const char *s1, char c, unsigned l);
int compara(const char *s1,const char *s2, unsigned l);

/*
lS = "12345"
ls = "55123"
*/


int fuerza_bruta2(const char *S, const char *s, unsigned lS, unsigned ls)
{
    
    unsigned int max = lS - ls;
    for(int i = 0; i <= max; i++){
        char* inicio = busca(S+i, s[0], lS-i);
        if(inicio == NULL){
            return -1;
        }
        int res = compara(inicio, s, ls);
        
        unsigned int pos = inicio-S;
        if(res == 1){
            return pos;
        }
        i = pos;
        
    
    }

    return -1;
}
