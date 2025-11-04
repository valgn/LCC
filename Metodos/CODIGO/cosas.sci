/* 
Un metodo iterativo converge si:
Primero fijate en si es digonal dominante que eso era viendo si 
la diagonal es mayor ESTRICTO a la sumatoria de todos los de la fila menos la diagonal
Como segundo resultado es si la norma de la matriz de iteracion es menor estricto a 1
Como segundo y ultimo recurso fijarse si el rango espectral de la matriz de iteracion es 
menor estricto a 1

Jacobi converge si A es simetrica definida positiva o si es diag dominante
Gauss Seidel lo mismo, generalmete converge mas rapido q Jacobi

Gorlami

Metodo de cholesky se puede usar si A es simetrica y def positiva
Ventajas que tiene es que usa la mitad de operaciones que LU

LU se puede usar si A es no singular

El metodo de la potencia te da el autovalor dominante y el autovector asociado

Gauss puede fallar si el pivote es 0
Si la matriz es estrictamente diagonal dominante o simetrica definida positiva, no hace
falta pivoteo

La facto QR se puede usar si las columnas de A son LI

*/
