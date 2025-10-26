function [x,a] = gausselim(A,B)
// Esta función obtiene la solución del sistema de ecuaciones lineales A*x=b, 
// dada la matriz de coeficientes A y el vector b.
// La función implementa el método de Eliminación Gaussiana sin pivoteo.  

    [nA,mA] = size(A) 
    [nb,mb] = size(B)
    
    if nA<>mA then 
        error('gausselim - La matriz A debe ser cuadrada');
        abort;
    elseif mA<>nb then
        error('gausselim - dimensiones incompatibles entre A y b');
        abort;
    end;
    
    a = [A B]; // Matriz aumentada

    // Eliminación progresiva
    n = nA;
    for k=1:n-1
        for i=k+1:n
                a(i,k+1:mA + mb) = a(i,k+1:mA + mb) - a(k,k+1:mA + mb) * a(i,k)/a(k,k)
                
                a(i,1:k) = 0                                    
        end;
    end;
    
    
    // Sustitución regresiva
    x(n,1:mb) = a(n,mA+1:mA + mb)/a(n,n);
    for i = 1:1:mb
        for j = n-1:-1:1
            x(j,i) = (a(j,i+mA) - a(j,j+1:n)*x(j+1:n,i))/a(j,j)
        end;
    end;
    // Calcular el determinante
    dete = 1
    for i=1:n
       dete = dete * a(i,i) 
    end
    disp(dete)
    disp(det(A))
    
endfunction

// Ejemplos de aplicación
A = [3 -2 -1; 6 -2 2; -9 7 1]
b = [0 6 -1]'

A = [1 2 3; 3 -2 1; 4 2 -1]
b = [14 9 -2; 2 -5 2; 5 19 12]

I = [1 0 0; 0 1 0; 0 0 1]


[x,a] = gausselim(A,I)
disp(x)
disp(a)


A2 = [0 2 3; 2 0 3; 8 16 -1]
b2 = [7 13 -3]'

A2 = [1 -1 2 -1; 2 -2 3 -3; 1 1 1 0; 1 -1 4 3]
b2 = [-8 -20 -2 4]'

//[x2,a2] = gausselim(A2,b2)--

// !--error 27 
//Division by zero...
//at line      24 of function gausselim called by :  
//[x2,a2] = gausselim(A2,b2)


