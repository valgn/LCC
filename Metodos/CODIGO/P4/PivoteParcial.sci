function [x,a] = gausselimPP(A,b)
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
    n = nA    // Tamaño de la matriz
    
    // Eliminación progresiva con pivoteo parcial
    for k = 1 : n-1
        kpivot = k // Comienza en la fila k
        amax = abs(a(k,k))
        
        // Con este for establezco en kpivot el número de fila con el máximo valor en la columna (debajo de k)
        for i = k+1 : n
            if abs(a(i,k)) > amax then
                kpivot = i
                amax = a(i,k)
            end
        end
        // Swapeo la fila k con la kpivot
        temp = a(kpivot,:)
        a(kpivot,:) = a(k,:)
        a(k,:) = temp
        
        // En estos dos for nos encargamos de hacer 0s en la columna actual debajo de la diagonal
        for i = k+1 : n
            for j = k+1 : n+1
                a(i,j) = a(i,j) - a(k,j)*a(i,k)/a(k,k)
            end
            for j=1:k        
                a(i,j) = 0 
            end       
        end
    end
    
    // Sustitución regresiva
    x(n) = a(n,n+1)/a(n,n)
    for i = n-1:-1:1
        sumk = 0
        for k=i+1:n
            sumk = sumk + a(i,k)*x(k)
        end
        x(i) = (a(i,n+1)-sumk)/a(i,i)
    end
endfunction

// Ejemplo de aplicación
A = [1 2 0 0;3 1 3 0;0 1 1 5;0 0 3 1]
b = [4 1 -3 4]' 

[x,a] = gausselimPP(A,b)
printf("Matriz aumentada escalonada:\n")
disp(a)
printf("Soluciones:\n")
disp(x)
