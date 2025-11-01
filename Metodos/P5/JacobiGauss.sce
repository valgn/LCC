// jacobi
// xi^{k+1} = 1/aii (bi - sum_{j=1, j/=i}^{n} aij xj^{k})

function [x, cont] = jacobi(A,b,x0,eps)
    n = size(A,1);
    x = x0;
    xk = x;
    suma = 0;
    cont = 0;
    
    for i=1:n
    suma = 0
        for j = 1:n
            if (i<>j)
                suma = suma + A(i,j)*xk(j)
            end
        end
    x(i) = 1/A(i,i)*(b(i)-suma)
    end
    cont = cont+1

    while (abs(norm(x-xk))> eps)
        xk = x;
        for i=1:n
            suma = 0
            for j = 1:n
                if (i<>j)
                    suma = suma + A(i,j)*xk(j)
                end
            end
            x(i) = 1/A(i,i)*(b(i)-suma)
        end
     cont = cont+1
    end
    disp(cont);
endfunction


// Gauss-Seidel
// xi^{k+1} = 1/aii (bi - sum_{j=1}^{i-1} aij xj^{k+1} 
//                      - sum_{j=i+1}^{n} aij xj^{k}
//                  )

function [x,cont] = gauss(A,b,x0,eps)
    n = size(A,1);
    x = x0;
    xk = x;
    suma = 0;
    cont = 0;
    
    for i=1:n
    suma = 0
        for j = 1:n
            if (i<>j)
                suma = suma + A(i,j)*x(j)
            end
        end
    x(i) = 1/A(i,i)*(b(i)-suma)
    end
    cont = cont+1

    while (abs(norm(x-xk))> eps)
        xk = x
        for i=1:n
            suma = 0
            for j = 1:n
                if (i<>j)
                    suma = suma + A(i,j)*x(j)
                end
            end
            x(i) = 1/A(i,i)*(b(i)-suma)
        end
     cont = cont+1
    end
    disp(cont);
endfunction

function diagonalDominante = esDiagonalDominanteGS(A)
    
    [nA,mA] = size(A);
    
    if (nA <> mA) then
        printf("Error - La matriz A debe ser cuadrada");
        abort;
    end
    
    diagonalDominante = %T;
    i = 1;
    
    while (i <= nA) && (diagonalDominante == %T)
        suma = sum(abs(A(i,:))) - abs(A(i,i));
        
        if abs(A(i,i)) <= suma then
            diagonalDominante = %F;
        end
        
        i = i + 1;
    end
    
endfunction

