function [x,cont] = sor(A,b,x0,w,eps)
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
    x(i) = (1-w)*x(i)+w/A(i,i)*(b(i)-suma)
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
            x(i) = (1-w)*x(i)+w/A(i,i)*(b(i)-suma)
        end
     cont = cont+1
    end

endfunction

function p=r_sor(w)
    p = sqrt(-(2/w-1)^2+1)
endfunction


// GAUSS SEIDEL CON MATRICES

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

function y = relaja(A, b, x0, e, w)
    x = x0
    D = diag(diag(A))
    N = D/w + tril(A) - D
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


A = [4 3 0; 3 4 -1; 1 -1 4]
b = [24 30 -24]'
x = [3 4 -5]'
[n,m] = size(A)
MatrizIt = (eye(n,n) - inv(diag(diag(A)))*A)
radioespec = max(abs(spec(MatrizIt)))
w = 2/(1+sqrt(1-radioespec^2))
[x, cont] = sor(A,b,x, w, 10^-7)
printf("Con el metodo de relajacion, con w optimo: \n")
printf("Cantidad de iteraciones: %d.\n", cont)
disp(x)

x = [3 4 -5]'
[x, cont] = sor(A,b,x, 1, 10^-7)
printf("Con el metodo de Gauss-Seidel, con w = 1: \n")
printf("Cantidad de iteraciones: %d.\n", cont)
disp(x)



