#include <stdlib.h>
#include <stdio.h>

int particionar(int* a, int len, int pivot){
    int j = 0;
    for(int i = 0; i < len; i++){
        if(a[i] <= pivot){
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            j++;
        }
    }
    return j;
}

void quickSort(int* a, int len){
    if(len < 2) return;
    int pivot = a[len-1];

    int pos = particionar(a, len - 1, pivot);
    int temp = a[pos];
    a[pos] = a[len - 1];
    a[len - 1] = temp;

    quickSort(a, pos);
    quickSort(a+pos+1, len-pos-1);
}

int main(){
    int a[10] = {3,4,6,2,5,9,10,11,4,6};
    quickSort(a, 10);
    for(int i = 0; i< 10; i++) printf("%d ", a[i]);
    return 0;
}