function [x,a] = gausselim_tridiagonal(A,b)
    [nA,mA] = size(A) 
    [nb,mb] = size(b)
    
    if nA<>mA then 
        error('gausselim - La matriz A debe ser cuadrada')
        abort
    elseif mA<>nb then
        error('gausselim - dimensiones incompatibles entre A y b')
        abort
    end
    
    a = [A b] // Matriz aumentada
    count = 0
    
    // Eliminación progresiva
    n = nA
    for k = 1 : n-1
        // Solo hacemos 0s en la subdiagonal
        a(k+1,k+1) = a(k+1,k+1) - a(k,k+1) * a(k+1,k)/a(k,k)
        a(k+1,n+1) = a(k+1,n+1) - a(k,n+1) * a(k+1,k)/a(k,k)
        a(k+1,k) = 0
        count = count + 1
    end
    
    // Sustitución regresiva
    x(n) = a(n,n+1)/a(n,n);
    for i = n-1:-1:1
        x(i) = (a(i,n+1)-a(i, i+1)*x(i+1))/a(i,i)
    end
    printf("Cantidad de operaciones por filas: %d\n", count)
endfunction

// Ejemplos de aplicación
A = [1 2 0 0; 3 1 3 0; 0 1 1 5; 0 0 3 1]
b = [4 1 -3 4]'

[x,a] = gausselim_tridiagonal(A,b)
printf("Matriz aumentada escalonada:\n")
disp(a)
printf("Soluciones:\n")
disp(x)


