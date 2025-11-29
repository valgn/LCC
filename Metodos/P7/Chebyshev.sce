function a = newtonCoef(x, y)
    n = length(x);
    a = y;
    for k = 2:n
        a(k:n) = (a(k:n) - a(k-1)) ./ (x(k:n) - x(k-1));
    end
endfunction

function y=poliNewton(x,y)
    n = length(x)
    a = newtonCoef(x,y)
    P = y(1)
    producto = 1;
    for k=2:n
        producto = producto * (poly(0, "x") - x(k-1))
        P = P + a(k) * producto
    end
    y = P
endfunction



function p=Cheby(x,n)
    if n == 0 then
        p = 1
    elseif n == 1 then
        p = x
    elseif n == 2 then
        p = 2*x^2-1
    else 
        p = 2*x*Cheby(x, n-1) - Cheby(x,n-2)
    end
endfunction

function r=rootsCheby(n)
    r = zeros(n,1) // Columna con las raices de Chebyshev
    for k = 0:n-1
        r(k+1,1) = cos((2*k+1) * %pi/(n*2))
    end
endfunction

function r=rootsChebyMap(n, a ,b)
    r = zeros(n,1) // Columna con las raices de Chebyshev
    for k = 0:n-1
        r(k+1,1) = cos((2*k+1) * %pi/(n*2))
    end
    r = ((b+a) - r*(b-a))/2
endfunction

// EJERCICIO 10 ----------------------------------------------
/*
nodos = rootsCheby(4) // Es columna
y = exp(nodos')
Poli = poliNewton(nodos', y)

rango = -1:0.001:1

subplot(211)
plot2d(rango, [exp(rango') horner(Poli, rango')], [6,3])
legend("e^x", "P3")
a = gca()
a.x_location = "origin"
a.y_location = "origin"
title("Interpolacion de e^x con nodos de Chebyshev")

// ERROR
function e = err(f, P, x)
    e = abs(f(x) - horner(P,x))
endfunction
errP3 = err(exp, Poli, rango)
subplot(212)
plot2d(rango, errP3', 2)
a = gca()
a.x_location = "origin"
a.y_location = "origin"
title("Error de P3 con e^x")
*/

// EJERCICIO 11 ----------------------------------------
function y=g(t)
    y = cos(t)
endfunction
// Como queremos aproximar en 0 a pi/2 (esta dentro de -1, 1), podemos usar el cambio de variable

nodos = rootsChebyMap(4, 0, %pi/2)' // El poli de Chebyshev de un grado mas porque se nos pide cubico
y = cos(nodos)'
poli = poliNewton(nodos, y')
rango = 0:0.001:%pi/2
subplot(211)
plot2d(rango, [cos(rango') horner(poli, rango')], [6,2])
legend("COS", "POLINOMIO")
a = gca()
a.x_location = "origin"
a.y_location = "origin"

function e = err(f, P, x)
    e = abs(f(x) - horner(P,x))
endfunction
erro = err(cos, poli, rango)
subplot(212)
plot2d(rango, erro', 2)
a = gca()
a.x_location = "origin"
a.y_location = "origin"
title("Error de P3 con e^x")
