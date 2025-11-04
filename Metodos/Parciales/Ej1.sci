function [U, ind, cont] = Cholesky(A)
    eps = 1.0e-8
    n = size(A,1)
    U = zeros(n,n)
    cont = 0
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
            cont = cont + 1 
        end
       
    end

    ind = 1
endfunction

function [A,A2]=armarMatriz(a)
    n = size(a,1)+1
    A = eye(n,n)
    A2 = eye(n,n)
    at = a'
    A(1, 2:n) = at
    A(2:n, 1) = a
    
    A2(1:n-1,n) = a
    A2(n,1:n-1) = at
endfunction


I = ones(400, 1)*1/4000


[A,A2]=armarMatriz(I)

tic()
[U2,ind2, cont2] = Cholesky(A2)
t2 = toc()

tic()
[U,ind, cont1] = Cholesky(A)
t1 = toc()

















