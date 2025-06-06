#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int* a, int len){
    int hice_algo = 1;
    while(hice_algo){
        hice_algo = 0;
        for(int i = 0; i<len-1; i++){
            if(a[i] > a[i+1]){
                int temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
                hice_algo = 1;
            }
        }
    }
}


int* insertion_sort(int* a, int len){
    for(int i = 1; i<len; i++){
        for(int j = i-1; j>=0; j--){
            if(a[j] > a[j+1]){
                int temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }
    return a;
}

void insertion_sort2(int* a, int len){
    for(int i = 1; i<len; i++){
        int key = a[i];
        int j = i-1;

        while(j>=0 && a[j] > key){
            a[j+1] = a[j];
            j--;
        }

        a[j+1] = key;
    }
}

int* selection_sort(int* a, int len){
    for(int i = 0; i<len-1; i++){
        int minimapos = i;
        for(int j = i+1; j<len; j++){
            if(a[j] < a[minimapos]){
                minimapos = j;
            }
        }
        int temp = a[i];
        a[i] = a[minimapos];
        a[minimapos] = temp;
    }
    return a;
}

void merge(int* a, int left, int medio, int right){
    int len1 = medio - left + 1;
    int len2 = right - medio;

    int izqarray[len1];
    int derarray[len2];

    for(int i = 0; i<len1; i++){
        izqarray[i] = a[left+i];
    }
    for(int j = 0; j<len2; j++){
        derarray[j] = a[medio + 1 + j];
    }
    //Unimos los arrays

    int i = 0, j = 0;
    int k = left;

    while(i < len1 && j < len2){
        if(izqarray[i] <= derarray[j]){
            a[k] = izqarray[i];
            i++;
        }
        else{
            a[k] = derarray[j];
            j++;
        }
        k++;
    }
    while(i < len1){
        a[k] = izqarray[i];
        i++;
        k++;
    }
    while(j < len2){
        a[k] = derarray[j];
        j++;
        k++;
    }
}

void mergesort(int* a, int left, int right){
    if(left < right){
        int medio = (left + right) / 2;
        mergesort(a, left, medio);
        mergesort(a, medio+1, right);

        merge(a, left, medio, right);

    }
}

/*
[3,2,1]
mergesort(a, 0, 2)
0<2
medio = 1;

mergesort(a, 0 , 1){
0<1
medio = 0
mergesort(a,0,0)
}
*/

int particionar(int* a,int len, int p){
    int j = 0;
    for(int i = 0; i < len; i++){
        if(a[i] <= p){
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            j++;
        }
    }
    return j;
}

void particion3ways(int* arr, int len, int pivote, int* finMenores, int* inicioMayores){
    int menor = 0;
    int igual = 0;
    int mayor = len - 1;

    while(igual <= mayor){
        if(arr[igual] < pivote){
            int temp = arr[igual];
            arr[igual] = arr[menor];
            arr[menor] = temp;

            menor++;
            igual++;
        }
        else if(arr[igual] > pivote){
            int temp = arr[igual];
            arr[igual] = arr[mayor];
            arr[mayor] = temp;
            mayor--;

        }
        else igual++;

    }
    *finMenores = menor;
    *inicioMayores = mayor;
}



void qisort(int* a, int len){
    if(len < 2){
        return;
    }
    int p = a[len-1]; //ultimo elemento

    int pos = particionar(a, len-1 , p);  

    int temp = a[pos];
    a[pos] = a[len - 1];
    a[len - 1] = temp;



    qisort(a, pos);
    qisort(a+pos+1, len-pos-1);
   

}

void qisort3ways(int* a, int len){
    if(len < 2){
        return;
    }
    int p = a[len-1]; //ultimo elemento

    int finMenores, inicioMayores;

    particion3ways(a, len, p, &finMenores, &inicioMayores);

    qisort3ways(a, finMenores);
    qisort3ways(a + inicioMayores + 1, len - inicioMayores - 1);

}


int binsearch(int a[], int len, int v){
    int posfin = len - 1;
    int posin = 0;

    while (posin <= posfin){
        int medio = (posfin + posin) / 2;
        if(v < a[medio]){
            posfin = medio - 1;
        }
        else if(v > a[medio]){
            posin = medio + 1;
        }
        else return medio;
    }
    return -1;
}





int main(){
    // time(NULL);

    int a[] = { 4, 9, 4, 4, 1, 9, 4, 4, 9, 4, 4, 1, 4 };
    int size = sizeof(a) / sizeof(int);
  

    // int b[100000];
    // for(int i = 0; i<100000; i++){
    //     int num = rand() % 100000;
    //     a[i] = num;
    //     b[i] = num;
    // }
    
    // clock_t start = clock();
    qisort3ways(a, size);
    // clock_t end = clock();

    // clock_t start1 = clock();
    // mergesort(b, 0, 100000-1);
    // clock_t end1 = clock();



    // double tiempo = (double)(end - start) / CLOCKS_PER_SEC;
    // double tiempo1 = (double)(end1 - start1) / CLOCKS_PER_SEC;

    // printf("Tiempo quicksort: %f segundos\nTiempo merge : %f segundos\n", tiempo, tiempo1);

    for(int i = 0; i<size; i++){
        printf("%d ", a[i]);
    }

    


    return 0;
}