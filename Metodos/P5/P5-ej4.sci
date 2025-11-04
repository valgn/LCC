
function [x,a] = gausselimPP(A,b)
    // Esta función obtiene la solución del sistema de ecuaciones lineales A*x=b, 
    // dada la matriz de coeficientes A y el vector b.
    // La función implementa el método de Eliminación Gaussiana con pivoteo parcial.

    [nA,mA] = size(A) 
    [nb,mb] = size(b)
    
    if nA<>mA then
        error('gausselim - La matriz A debe ser cuadrada');
        abort;
    elseif mA<>nb then
        error('gausselim - dimensiones incompatibles entre A y b');
        abort;
    end;
    
    a = [A b]; // Matriz aumentada
    n = nA;    // Tamaño de la matriz
    
    // Eliminación progresiva con pivoteo parcial
    for k=1:n-1
        kpivot = k; amax = abs(a(k,k));  //pivoteo
        for i=k+1:n
            if abs(a(i,k))>amax then
                kpivot = i; amax = a(i,k);
            end;
        end;
        temp = a(kpivot,:); a(kpivot,:) = a(k,:); a(k,:) = temp;
        
        for i=k+1:n
            for j=k+1:n+1
                a(i,j) = a(i,j) - a(k,j)*a(i,k)/a(k,k);
            end;
            for j=1:k        
                a(i,j) = 0;  
            end              
        end;
    end;
    
    // Sustitución regresiva
    x(n) = a(n,n+1)/a(n,n);
    for i = n-1:-1:1
        sumk = 0
        for k=i+1:n
            sumk = sumk + a(i,k)*x(k);
        end;
        x(i) = (a(i,n+1)-sumk)/a(i,i);
    end;
endfunction

function x = gauss(A,b,x0,eps)
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
    // disp(cont);
endfunction

N = 500
A = 8*eye(N,N) + 2*diag(ones(N-1,1),1) + 2*diag(ones(N-1,1),-1) + diag(ones(N-3,1),3) + diag(ones(N-3,1),-3)

b = ones(N,1)

// GAUSS_SEIDEL
x0 = zeros(N,1)
tic();
gauss(A,b, x0, 10^-6)
t = toc();
printf("Gauss-Seidel tarda %d segundos para N = %d.\n", t, N);

// Pivoteo
tic();

gausselimPP(A,b)
t = toc()
printf("Pivoteo parcial tarda %d segundos para N = %d.\n", t, N)










