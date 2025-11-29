// Parcial 2022 Tema 2
// f(x,v) = a + bx + cv
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
//Datos
xp = [0,0,1,2,2,2]
vp = [0,1,0,0,1,2]
yp = [1.42, 1.85, 0.78, 0.18, 0.60, 1.05]'

// Hay que encontrar 3 coeficientes por lo que A*x = b 

function y=CrearMatriz(x,v)
    n = length(x)
    A = zeros(n,3)
    A(:,1) = ones(n,1)'
    A(:,2) = x'
    A(:,3) = v'
    y=A
endfunction

A = CrearMatriz(xp,vp)
w = gausselimPP(A'*A, A'*yp)
scatter3d(xp, vp, yp, ); 


Z_plano = zeros(3,3); 

for i = 1:3
    for j = 1:3
        // f(x,v) = a + bx + cv
        Z_plano(i,j) = w(1) + w(2)*[0,1,2](i) + w(3)*[0,1,2](j);
    end
end


set(gca(), "auto_clear", "off") 
plot3d([0,1,2],[0,1,2] ,Z_plano);

// EJ 2


function w = SimpsonComp(a,b,f,n)
    h = (b-a)/n
    suma = f(a) + f(b)
    for k=1:n-1
        if (modulo(k,2)==0) then
            suma = suma + 2*f(a + k*h)
        else
            suma = suma + 4*f(a+k*h)
        end
    end
    w = h/3*suma
endfunction

function y=f(x)
    y = exp(x)*sin(x)
endfunction
I = SimpsonComp(1,3,f,16)

