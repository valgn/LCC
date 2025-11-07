// Ejercicio 10, Doolittle

// Recordar que A = LU => Ax = b = LUx = b

function y = superior(A,b)
    [n, m] = size(A)
    x = zeros(n,1)
    x(n) = b(n)/A(n,m)
    for i = n-1:-1:1
        x(i) = (b(i) - A(i,i+1:n)*x(i+1:n))/A(i,i)
    end
    y = x
endfunction

// Resuelve un sistema triangular inferior con sustitución progresiva¿?
function y = inferior(A,b)
    [n,m] = size(A)
    x = zeros(n,1)
    x(1) = b(1)/A(1,1)
    for i = 2:1:n
        x(i) = (b(i) - A(i, 1:i)*x(1:i))/A(i,i)
    end
    y = x
endfunction

function [U,L,y] = Doolittle(A,b)
    n = size(A,1)
    U = zeros(n,n)
    L = eye(n,n)
    
    for i=1:n
        for j=1:n
            if i <= j then
                U(i,j) = A(i,j) - sum(L(i,1:i-1)*U(1:i-1,j))

            else i > j
                L(i,j) = (A(i,j) - sum(L(i,1:j-1)*U(1:j-1,j)))/U(j,j)
            end
        end
    end
    // LUx = b, Ux = y = y, Ly = b
    y = inferior(L, b)
    x = superior(U, y)
    y=x
    
endfunction

// --------------------------------------------------------------------
// Doolittle con matriz P permutacion

function [P, L, U, x] = DoolittleP(A, b)
    [n, m] = size(A);
    if n <> m then
        error("La matriz A debe ser cuadrada");
    end

    // Inicializaciones
    L = eye(n, n);
    U = A;           // iremos transformando U
    P = eye(n, n);   // matriz de permutación

    // Factorización con pivoteo parcial
    for k = 1:n-1
        // Buscar fila con mayor valor absoluto en columna k (desde k a n)
        [ma, idx] = max(abs(U(k:n, k)));
        kpivot = idx + k - 1;

        // Si hay que pivotear, intercambiamos filas en U, P y L(1:k-1)
        if kpivot <> k then
            // intercambiar filas kpivot y k en U
            temp = U(kpivot, :);
            U(kpivot, :) = U(k, :);
            U(k, :) = temp;

            // intercambiar filas kpivot y k en P
            temp = P(kpivot, :);
            P(kpivot, :) = P(k, :);
            P(k, :) = temp;

            // intercambiar filas kpivot y k en L, pero solo las columnas 1:(k-1)
            // (los multiplicadores ya calculados)
            if k > 1 then
                temp = L(kpivot, 1:k-1);
                L(kpivot, 1:k-1) = L(k, 1:k-1);
                L(k, 1:k-1) = temp;
            end
        end

        // Comprobación de pivote nulo (o muy pequeño)
        if abs(U(k, k)) < %eps then
            error("Pivote nulo o demasiado pequeño. Factorización falla.");
        end

        // Eliminación (Doolittle): calcular L(i,k) y actualizar U
        for i = k+1:n
            L(i, k) = U(i, k) / U(k, k);
            U(i, k:n) = U(i, k:n) - L(i, k) * U(k, k:n);
        end
    end

    // Resolver el sistema usando P, L, U
    Pb = P * b;
    y = inferior(L, Pb);   // sustitución progresiva (Ly = Pb)
    x = superior(U, y);    // sustitución regresiva (Ux = y)
endfunction

// Te calcula Crout
function [L,U,x] = Crout(A,b)
    n = size(A,1)
    L = zeros(n,n)
    U = eye(n,n) // diagonal de 1s

    for j = 1:n
        // Calcular la columna j de L
        for i = j:n
            L(i,j) = A(i,j) - sum(L(i,1:j-1) .* U(1:j-1,j))
        end

        // Calcular la fila j de U
        for k = j+1:n
            U(j,k) = (A(j,k) - sum(L(j,1:j-1) .* U(1:j-1,k))) / L(j,j)
        end
    end

    // Resolver el sistema LUx = b
    // 1) Ly = b (sustitución progresiva)
    y = inferior(L, b)
    // 2) Ux = y (sustitución regresiva)
    x = superior(U, y)
endfunction


A = [1 2 3 4;1 4 9 16;1 8 27 64;1 16 81 256]

b = [2;10;44;190]


