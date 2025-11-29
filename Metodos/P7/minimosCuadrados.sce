// EJERCICIO 7
/*
x = 0, 0.15, 0.31, 0.5, 0.6, 0.75
y = 1, 1.004, 1.31, 1.117, 1.223, 1.422

grado 1: a0 + a1x
grado 2: a0 + a1x + a2x^2
grado 3: a0 + a1x + a2x^2 + a3x^3

*/

x = [0, 0.15, 0.31, 0.5, 0.6, 0.75]'
y = [1, 1.004, 1.031, 1.117, 1.223, 1.422]'

// CONSTRUIR A ------------------------------
function A=construirA(x, p) // Conjunto de puntos y grado de pol
    n = length(x)
    A = zeros(n, p+1)
    for i=0:p
        A(: , i + 1) = x.^i
    end
endfunction
// CONSTRUIR A ------------------------------

// --------------------------------------------- Gausselim PP
function x = gausselimPP(A,b)
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
// --------------------------------------------- Gausselim PP

// --------------------------------------------- QR
function [Q,R] = QR(A)
    
    [m,n] = size(A);
    

    if rank(A) <> n  then
        error("factorizacionQR - El rango de la matriz no es n");
    end

    Q = zeros(m,n);
    R = zeros(n,n);
    
    for k = 1 : n // Iteramos por las columnas de la matriz
        
        vectorProyectado = A(1:m, k);
        
        for i = 1 : k - 1 
            
            R(i,k) = A(1:m, k)' * Q(1:m, i);
            
            vectorProyectado = vectorProyectado - R(i,k) * Q(1:m, i);
        end
        
        Q(1:m, k) = vectorProyectado / norm(vectorProyectado);

        R(k,k) = norm(vectorProyectado);

        
    end
// --------------------------------------- QR    
endfunction


function x = triangularSuperior(A,b)
    
    [nA,mA] = size(A);

    x(nA) = b(nA)/A(nA,nA);
    
    for i=nA-1:-1:1
        x(i)= (b(i) - A(i,i+1:nA)*x(i+1:nA))/A(i,i); // A(i,i+1:n) fila i columna i+1 a n
    end
    
endfunction

// Si la matriz no es invertible, su numero de condicion es infinito (esta mal condicionada)
function [w, poli, err]=minimoscuad(A, y)
        // QR asegura mas estabilidad antes que A'*Ax = A'*b
        [Q,R] = QR(A)
        w = triangularSuperior(R, Q'*y)
        poli = poly(w, "x", "coeff")
        err = norm(A*w-y)
endfunction



//Grado 1
A1 = construirA(x, 1)
[w1, poli1, err1] = minimoscuad(A1, y)

//Grado 2
A2 = construirA(x, 2)
[w2, poli2, err2] = minimoscuad(A2, y)

//Grado 3
A3 = construirA(x, 3)
[w3, poli3, err3] = minimoscuad(A3, y)

/*
printf("El error relativo del de grado 3.\n")
disp(norm(A3*w3 - y) / norm(y))
*/

// EJERCICIO 8
x = [4 4.2 4.5 4.7 5.1 5.5 5.9 6.3 6.8 7.1]';
y = [102.56 113.18 130.11 142.05 167.53 195.14 224.87 256.73 299.5 326.72]';
A4 = construirA(x, 1)
[w4, poli4, err4] = minimoscuad(A4, y)

//Grado 2
A5 = construirA(x, 2)
[w5, poli5, err5] = minimoscuad(A5, y)

//Grado 3
A6 = construirA(x, 3)
[w6, poli6, err6] = minimoscuad(A6, y)

// Plot
rango = 4:0.001:7.2
plot2d(x',y,-1);
plot2d(rango,[horner(poli4,rango') horner(poli5,rango') horner(poli6,rango')], [2,3,5],leg=["px1(x)@px2(x)@px3(x)"]);


