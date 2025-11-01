function [x,a] = gausselim(A,b)
    // Esta función obtiene la solución del sistema de ecuaciones lineales A*x=b, 
    // dada la matriz de coeficientes A y el vector b.
    // La función implementa eliminación Gaussiana sin pivoteo.  

    [nA, mA] = size(A) 
    [nb, mb] = size(b)
    
    if nA<>mA then 
        error('gausselim - La matriz A debe ser cuadrada');
        abort;
    elseif mA<>nb then
        error('gausselim - dimensiones incompatibles entre A y b');
        abort;
    end;
    
    a = [A b]; // Matriz aumentada
    s = 0
    
    // Eliminación progresiva
    n = nA;
    for k=1:n-1
        for i=k+1:n
            a(i,k+1:n+1) = a(i,k+1:n+1) - a(k,k+1:n+1) * a(i,k)/a(k,k)
            a(i,1:k) = 0                                    
        end;
    end;
    
    // Sustitución regresiva
    x(n) = a(n,n+1)/a(n,n);
    for i = n-1:-1:1
        sumk = a(i,i+1:n)*x(i+1:n)
        x(i) = (a(i,n+1)-sumk/a(i,i));
    end;
    
endfunction

// Ejemplos de aplicación 
A = [3 -2 -1; 6 -2 2; -9 7 1]
b = [0 6 -1]'

[x,a] = gausselim(A,b)
printf("Matriz escalonada aumentada:\n")
disp(a)
printf("Soluciones:\n")
disp(x)

