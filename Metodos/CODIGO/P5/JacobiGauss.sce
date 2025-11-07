// JACOBI, GAUSS, DIAGONAL DOMINANTE


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

// GAuss Seidel Matricial
function y = GS(A, b, x0, e)
    x = x0
    N = tril(A) 
    Ninversa = inv(N)
    xk = Ninversa*((N-A)*x0 + b)
    count = 1
    
    while abs(norm(A*x-b)) > e
        x = Ninversa*((N-A)*xk + b)
        xk = x
        count = count + 1
    end
    //disp(count)
    y = x
endfunction

// Jacobi Matricial

function y = Jac(A, b, x0, e)
    x = x0
    N = diag(diag(A)) 
    Ninversa = inv(N)
    xk = Ninversa*((N-A)*x0 + b)
    count = 1
    
    while abs(norm(A*x-b)) > e
        x = Ninversa*((N-A)*xk + b)
        xk = x
        count = count + 1
    end
    //disp(count)
    y = x
endfunction

// SOR CON MATRICES

function y = SOR(A, b, x0, e, w)
    // A: matriz del sistema
    // b: vector del sistema
    // x0: vector inicial
    // e: tolerancia
    // w: parámetro de relajación (0 < w < 2)

    x = x0
    D = diag(diag(A))
    L = tril(A) - D
    U = triu(A) - D

    // matriz de iteración y vector constante
    N = D + w*L
    Ninversa = inv(N)

    xk = Ninversa * (w*b - ((w*U + (w-1)*D) * x0))
    count = 1

    while norm(A*x - b) > e
        x = Ninversa * (w*b - ((w*U + (w-1)*D) * xk))
        xk = x
        count = count + 1
    end

    //disp(count)
    y = x
endfunction

