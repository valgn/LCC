function [U,L,P] = factorizacion_LU(A)
    [n,m] = size(A) 
    
    if n <> m then
        error("La matriz no es cuadrada.")
    end
    
    U = A
    L = eye(n, n)
    P = eye(n, n)
    
    for k = 1 : n-1
        kpivot = k
        amax = abs(U(k,k))
        for i = k+1 : n
            if abs(U(i,k)) > amax then
                kpivot = i
                amax = U(i,k)
            end
        end
        
        // Swapeo las filas de U
        temp = U(kpivot,:)
        U(kpivot,:) = U(k,:)
        U(k,:) = temp
        
        // Swapeo las filas de L
        temp = L(kpivot,1:k-1)
        L(kpivot,1:k-1) = L(k,1:k-1)
        L(k,1:k-1) = temp
        
        // Swapeo las filas de P
        temp = P(kpivot, :)
        P(kpivot,:) = P(k,:)
        P(k,:) = temp
        
        for j = k+1 : m
            L(j, k) = U(j,k)/U(k,k)
            U(j, k:m) = U(j,k:m) - (L(j,k)*U(k,k:m))
        end
   end
endfunction

// Ejemplo de aplicación
A = [2 1 1 0; 4 3 3 1; 8 7 9 5; 6 7 9 8]

[U,L,P] = factorizacion_LU(A)
printf("P:\n")
disp(P)
printf("L:\n")
disp(L)
printf("U:\n")
disp(U)
printf("P*A:\n")
disp(P*A)
printf("L*U:\n")
disp(L*U)
