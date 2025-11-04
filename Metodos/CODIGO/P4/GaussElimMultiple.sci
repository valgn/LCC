function [x,a] = gausselim_multiples(A,B)
    // Recibimos varios sistemas A*x = b donde A siempre es igual y b varía
    // Retornamos la solución de cada sistema
    
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
    
    // Calcular el determinante (EJ 4)
    dete = 1
    for i = 1:n
       dete = dete * a(i,i) 
    end
    printf("Determinante: %f\n", dete)
    
endfunction

// Ejemplos de aplicación
A = [1 2 3; 3 -2 1; 4 2 -1]
B = [14 9 -2; 2 -5 2; 5 19 12]

[x,a] = gausselim_multiples(A,B)
printf("Matriz aumentada escalonada:\n")
disp(a)
printf("Soluciones:\n")
disp(x)

// Ej 3 c (calcular la matriz inversa de A atraves del método) 
// A*X = B => A*X = I debería retornar la inversa de A

// Ejemplos de aplicación
I = [1 0 0; 0 1 0; 0 0 1]

[x,a] = gausselim_multiples(A,I)
printf("Matriz aumentada escalonada:\n")
disp(a)
printf("Soluciones:\n")
disp(x)



