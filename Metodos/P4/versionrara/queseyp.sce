function [U,L,P] = Facto(A)
    // Esta función obtiene la solución del sistema de ecuaciones lineales A*x=b, 
    // dada la matriz de coeficientes A y el vector b.
    // La función implementa el método de Eliminación Gaussiana con pivoteo parcial.
    
    [n,m] = size(A) 
     
    
    
    
    if n <> m then
        error("La matriz no es cuadrada.")
    end
    
    U = A
    L = eye(n, n)
    P = eye(n, n)
    
    // Eliminación progresiva con pivoteo parcial
    for k=1:n-1
        kpivot = k; amax = abs(U(k,k));  //pivoteo
        for i=k+1:n
            if abs(U(i,k))>amax then
                kpivot = i; amax = U(i,k);
            end;
        end;
    
        
        temp = U(kpivot,:); 
        U(kpivot,:) = U(k,:); 
        U(k,:) = temp;
        
        temp = L(kpivot,1:k-1); 
        L(kpivot,1:k-1) = L(k,1:k-1); 
        L(k,1:k-1) = temp;
        
        temp = P(kpivot, :)
        P(kpivot,:) = P(k,:)
        P(k,:) = temp
        
        for j=k+1:m
            L(j,k) = U(j,k)/U(k,k)
            U(j,k:m) = U(j,k:m) - (L(j,k)*U(k,k:m))
        end
    
   end;
endfunction

// Ejemplo de aplicación
A = [2 1 1 0;4 3 3 1;8 7 9 5;6 7 9 8]
//b = [4 1 -3 4]' 

[U,L,P] = Facto(A)


disp(P*A)
disp(L*U)
