function y = triangular(A,b)
    [n,m] = size(A)
    x = zeros(n,1)
    x(n) = b(n)/A(n,m)
    for i = n-1 : -1 : 1
        x(i) = (b(i) - A(i,i+1:n)*x(i+1:n))/A(i,i)
    end
    y = x
endfunction

function y = inferior(A,b)
    [n,m] = size(A)
    x = zeros(n,1)
    x(1) = b(1)/A(1,1)
    for i = 2 : 1 : n
        x(i) = (b(i) - A(i, 1:i)*x(1:i))/A(i,i)
    end
    y = x
endfunction

function [U,P,L,x] = gauss_LU(A,b)
    [n,m] = size(A) 
    
    if n <> m then
        error("La matriz no es cuadrada.")
    end
    
    U = A
    L = eye(n,n)
    P = eye(n, n)
    
    // Obtengo las matrices P, L y U
    for k = 1 : n-1
        kpivot = k
        amax = abs(U(k,k)) //pivoteo
        for i = k+1 : n
            if abs(U(i,k)) > amax then
                kpivot = i 
                amax = U(i,k)
            end
        end
    
        if amax <> U(k,k) then
            temp = U(kpivot,:)
            U(kpivot,:) = U(k,:)
            U(k,:) = temp
            
            temp = L(kpivot,1:k-1)
            L(kpivot,1:k-1) = L(k,1:k-1)
            L(k,1:k-1) = temp
            
            temp = P(kpivot, :)
            P(kpivot,:) = P(k,:)
            P(k,:) = temp
        end;
        
        for j=k+1:m
            L(j,k) = U(j,k)/U(k,k)
            U(j,k:m) = U(j,k:m) - (L(j,k)*U(k,k:m))
        end
    end;
    
    // Partimos de Ax = b, luego PAx = Pb
    // Ahora como PA = LU tenemos LUx = Pb
    // Entonces reagrupamos L(Ux) = Pb
    // Llamamos y = Ux entonces nos queda el sistema triangular inferior Ly = Pb y resolvemos
    // Luego tenemos Ux = y y resolvemos el sistema triangular superior conociendo y 
    Pb = P*b
    y = inferior(L, Pb)
    x = triangular(U, y)    
        
endfunction

// Ejemplo de aplicación
A = [1 2 -2 1; 4 5 -7 6; 5 25 -15 -3; 6 -12 -6 22]
b = [2 2 1 0]' 

[U, P, L, x] = gauss_LU(A,b)

printf("Matriz P:\n")
disp(P)
printf("Matriz L:\n")
disp(L)
printf("Matriz U:\n")
disp(U)
printf("Soluciones:\n")
disp(x)

