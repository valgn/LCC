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
    x = triangular(U, y)
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

A = [16 -12 8; -12 18 -6; 8 -6 8]
[Q, R] = GramSchmidt(A)

A = [16 -12 8; -12 18 -6; 8 -6 8]
[Q, R] = GramSchmidt(A)


// A = [16 -12 8 -16; -12 18 -6 9;8 -6 5 -10; -16 9 -10 46]
// B =[4 1 1;8 2 2;1 2 3]
// C = [1 2;2 4]

//[U1,ind1] = Cholesky(A)
//disp(U1)
//disp(ind1)


//[U2,ind2] = Cholesky(B)
//disp(U2)
//disp(ind2)


//[U3,ind3] = Cholesky(C)
//disp(U3)
//disp(ind3)

A =[16 -12 8; -12 18 -6;8 -6 8]
b = [76, -66 46]


