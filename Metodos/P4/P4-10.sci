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



A = [1 2 3 4;1 4 9 16;1 8 27 64;1 16 81 256]

b = [2;10;44;190]


