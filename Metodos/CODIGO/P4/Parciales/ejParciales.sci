
function y = superior(A,b)
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


function r=definidaPositiva(A)
    r = %T
    autoval = spec(A)
    for i=1:size(autoval, 1)
        if(real(autoval(i)) <= 0)
            r = %F
            break
        end
    end
    
endfunction

function r=esSimetrica(A)
    if(A == A') then
        r = %T
    else 
        r = %F
    end
endfunction
// Ej 1  ITEM A
A = [6 2 1 0 1; 2 5 2 1 0; 1 2 6 2 1; 0 1 2 5 2; 1 0 1 2 4]
function r=a(A)
    r = esSimetrica(A) & definidaPositiva(A)
endfunction

// ITEM B
function [U, ind] = Cholesky(A)
eps = 1.0e-8
n = size(A,1)
U = zeros(n,n)
for k = 1:n
    if k==1 then
            t = A(k,k)
    else 
            t = A(k,k) - U(1:k-1,k)'*U(1:k-1,k)
    end

    if t <= eps
        printf("Matriz no definida positiva.\n")
        ind = 0
        return
    end
    U(k,k)= sqrt(t)
    for j = k+1:n
        if k==1 then 
                    U(k,j) = A(k,j)/U(k,k)
        else 
                    U(k,j) = ( A(k,j) - U(1:k-1,k)' * U(1:k-1,j) )/U(k,k)
        end
    end
end
ind = 1
endfunction

function y= CholeskyRes(A, b)
    [U, ind] = Cholesky(A)
    UT = U'
    // Ax = b, U'Ux = b
    y = inferior(UT, b)
    x = superior(U, y)
    y = x

endfunction

function [Q,R] = GramSchmidt(A) // A tiene que tener vectores li en cols
    [n,m] = size(A)
    
    Q = zeros(n,m)
    Q(1:n, 1) = A(1:n, 1)/norm(A(1:n, 1))
    
    R = zeros(n,n)
    R(1,1) = norm(A(1:n, 1))
    
    for i=1:n
        for j=2:n
            vL = A(1:n, j)
            for k = 1: j-1
                vL = vL - (A(1:n, j)'*Q(1:n, k))*Q(1:n,k)
            end
            Q(1:n, j) = (vL)/norm(vL)
            R(j,j) = norm(vL)
            if j > i then
                R(i,j) = A(1:n, j)'*Q(1:n, i)
            end
        end
    end
end

function [Q,R,U, ind]=b(A)
    [Q,R] = GramSchmidt(A)
    [U, ind] = Cholesky(A)
    
endfunction

function [xchol, xqr]=c(Q, R, U, b)
    Ut = U'
    nb = Q'*b
    xqr = superior(R, nb)
    
    ychol = inferior(Ut, b)
    xchol = superior(U, ychol) // Calcula la solucion con Cholesky, el de arriba con QR  
endfunction

b = [6 5 4 1 0]
function [err1,err2] = d(A, xchol, xqr, b)
    err1 = norm(A*xchol - b)
    err2 = norm(A*xqr - b)
    if (err1 < err2) then
        printf("Cholesky aproxima mejor.\n")
    elseif (err2 < err1) then
        printf("QR aproxima mejor.\n")
    else
        printf("E lo mimo.\n")
    end
endfunction

// Parcial no se cuanto Practica 5

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
    disp(count)
    y = x
endfunction

function [A,b] =armarMatriz(n)
    A = zeros(n,n)
    b = zeros(n, 1)
    b(n, 1) = 1

    I = eye(n,n)
    D = diag(diag(I)) * -2
    L = (diag(diag(A, -1)-1,-1) )
    U = (diag(diag(A, 1)-1, 1) )
    A = D + L + U
    A(1, n) = 1
    A(n,1) = 1
endfunction

function gs=matrizIte(A)
    L = tril(A)
    Linv = inv(L)
    I = eye(size(A,1),size(A,1))
    gs = I-Linv*A
endfunction

// Practica 4
A = [1 2 -3 4; 4 8 12 -8; 2 3 2 1; -3 -1 1 -4]
b = [3 60 1 5]'
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
    x = superior(U, y)            
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





[x, a] = gausselimPP(A, b)





