#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "btree.h"

#define WALL 1

void print_it(int a){
    printf("%d ", a);
}



void solve_maze_helper(int maze[10][10], int visited[10][10]){
    int n = 0, m = 0, notTrapped = 1, notFinished = 1;
    int left, right, top, bottom;
    printf("[ ");
    while(notTrapped && notFinished){
        right = (m + 1 < 10) ? maze[n][m + 1] : WALL;
        left = (0 < m - 1) ? maze[n][m - 1] : WALL;
        top = (n + 1 < 10) ? maze[n + 1][m] : WALL;
        bottom = (0 < n - 1) ? maze[n - 1][m] : WALL;
        visited[m][n] = 1;
        if(right != WALL && !visited[n][m + 1]){
            m++;
        }
        else if(bottom != WALL && !visited[n - 1][m]){
            n--;
        }
        else if(left != WALL && !visited[n][m - 1]){
            m--;
        }
        else if(top != WALL && !visited[n + 1][m]){
            n++;
        }

        printf("(%d, %d), ", n, m);
        if(n == 9 && m == 9){
            printf("I finished!\n");
            notFinished = 0;
        }
        else if (n == 0 && m == 0){
            printf("I'm trapped.\n");
            notTrapped = 0;
        }
    }
    printf("\b\b ]");
}

void solve_maze(int maze[10][10]){ 
    int visited[10][10];
    int m, n;
    for(m = 0; m < 10; m++)
        for(n = 0; n < 10; n++)
            visited[m][n] = 0;
    solve_maze_helper(maze, visited);
}

int main(){
    printf("hello world\n");
    BTree arbol = btree_crear();
    BTree abb = btree_crear();
    int m;   

    ABB_insertar(40, abb);

    for (int i = 0; i < 44; i++){
        m = rand() % 100;
        arbol = btree_insertar_random(m, arbol);
        abb = ABB_insertar(m, abb);
    }

    printf("\"arbol\" es abb?: %d\n", es_ABB(arbol));
    printf("\"abb\" es abb?: %d\n", es_ABB(abb));
    
    printf("Eliminando 40 de abb...\n");
    eliminar_ABB(40, abb);
    assert(es_ABB(abb));

}   