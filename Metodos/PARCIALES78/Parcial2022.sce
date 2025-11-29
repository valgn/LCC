// Parcial 2022 Tema 1.

warning('off')
function A=construirA(x, p) // Conjunto de puntos y grado de pol
    n = length(x)
    A = zeros(n, p+1)
    for i=0:p
        A(: , i + 1) = x.^i
    end
endfunction

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

function x = triangularSuperior(A,b)
    
    [nA,mA] = size(A);

    x(nA) = b(nA)/A(nA,nA);
    
    for i=nA-1:-1:1
        x(i)= (b(i) - A(i,i+1:nA)*x(i+1:nA))/A(i,i); // A(i,i+1:n) fila i columna i+1 a n
    end
    
endfunction

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

function [w, poli, err]=minimoscuad(A, y)
        // 
        [Q,R] = qr(A, "e")
        w = inv(R)*Q'*y
        poli = poly(w, "x", "coeff")
        err = norm(A*w-y)
endfunction

function [w,poli,err]=MinimosInv(A,y)
    w = inv(A'*A)*(A'*y)
    poli = poly(w, "x", "coeff")
    err = norm(A*w-y)
endfunction
x = 1:10
ymaxTemp = [32.9, 30.8, 26.4, 24.2, 19.2, 16.5, 19.3, 21, 23, 26.2]'
A3 = construirA(x, 3)
[w3,p3, err3] = MinimosInv(A3, ymaxTemp)

A5 = construirA(x, 5)
[w5,p5, err5] = MinimosInv(A5, ymaxTemp)

A7 = construirA(x, 7)
[w7,p7, err7] = MinimosInv(A7, ymaxTemp)

A9 = construirA(x, 9)
[w9,p9, err9] = MinimosInv(A9, ymaxTemp)

subplot(212)
rango = 0:0.1:10
plot2d(x, ymaxTemp, -1)
plot2d(rango, [horner(p3, rango') horner(p5, rango') horner(p7, rango') horner(p9, rango')], [1,3,4,5])
legend("Puntos", "P3", "P5", "P7", "P9")
title("Grafico hecho con matriz Inversa de AT*A")
// QR -----------------------------------

A3 = construirA(x, 3)
[w3,p3, err3] = minimoscuad(A3, ymaxTemp)

A5 = construirA(x, 5)
[w5,p5, err5] = minimoscuad(A5, ymaxTemp)

A7 = construirA(x, 7)
[w7,p7, err7] = minimoscuad(A7, ymaxTemp)

A9 = construirA(x, 9)
[w9,p9, err9] = minimoscuad(A9, ymaxTemp)

subplot(211)
rango = 0:0.1:10
plot2d(x, ymaxTemp, -1)
plot2d(rango, [horner(p3, rango') horner(p5, rango') horner(p7, rango') horner(p9, rango')], [1,3,4,5])
legend("Puntos", "P3", "P5", "P7", "P9")
title("Grafico hecho con QR")

// Practica Integracion NUmerica
function y=f1(x)
    y = 1/x
endfunction

function w = TrapecioComp(a,b,f,n)
    h = (b-a)/n
    suma = 1/2*f(a) + 1/2*f(b)
    for k=1:n-1
        suma = suma + f(a + k*h)
    end
    suma = h*suma
    w = suma
endfunction

aprox = TrapecioComp(1,2,f1,4)
// Sabemos que la formula del error de la formula trapezoidal es
/*
-h^2(b-a)/12 * f''(c) para algun c entre a y b
En este caso, h = b-a/n => h = 1/4, como queremos saber cuantos intervalos necesitamos, generalizamos para n
entonces h = 1/n. De esta forma: -((1/n)^2*1)/12 * f''(c)
- (1/12*n^2) * f''(c) = e
f(x) = 1/x
f' = -1/x^2
f'' = 2/x^3 (Funcion decreciente en [1,2]), podemos escoger 1
|e| <= 1/12*n^2 * 2
Como queremos que |e| <= 10^-6

1/6*n^2  <= 1/10^6
6*n^2 >= 10^6
n^2 >= 10^6/6
n >= sqrt(10^6/6) COMPROBAMOS...
*/
err = abs(log(2) - TrapecioComp(1,2,f1, 289))
disp(err <= 10^-6)





