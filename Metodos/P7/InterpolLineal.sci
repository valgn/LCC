function p = Lk(x,k)
    
    nX = length(x); // tamaño del vector de nodos x
    p = 1; // neutro del producto (polinomio constante 1)
    xk = x(k); // nodo k
    xx = poly(0,"x"); // x
    
    for i = 1:nX
        
        if (i<>k) then
            p = p * (xx - x(i))/(xk-x(i)); // Lk
        end
        
    end
    
endfunction



function P = lagrangePoly(x, y)
    n = length(x);
    P = 0;

    for k = 1:n
        // Construyo L_k(x) como polinomio
        Lk = 1;   // arranca como el polinomio constante 1
        
        for j = 1:n
            if j <> k then
                Lk = Lk * (poly(0,"x") - x(j)) / (x(k) - x(j));
            end
        end
        P = P + y(k)*Lk;
    end
endfunction

function graficoInterpolacion(Plineal, Pcubica)
    clf();
    rango = -2:0.01:2;

    // Evaluaciones
    y_lineal = horner(Plineal, rango);
    y_cubica = horner(Pcubica, rango);
    y_real   = exp(rango);

    // Graficar
    plot(rango, y_lineal, "r");
    plot(rango, y_cubica, "b");
    plot(rango, y_real, "g");

    a = gca();
    a.x_location = "origin";
    a.y_location = "origin";
    a.isoview = "off";
    a.box = "on";

    legend("Interpolación lineal", "Interpolación cúbica", "e^x");
endfunction

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

x1=[0,0.2];
y1=[1,1.2214];

x2=[0,0.2,0.4,0.6];
y2=[1,1.2214,1.4918,1.8221];

Plineal = lagrangePoly(x1,y1);
Pcubica = lagrangePoly(x2,y2);

PlinealN = poliNewton([0,0.2], [1,1.2214])
PcubicaN = poliNewton([0,0.2,0.4,0.6], [1,1.2214,1.4918,1.8221])

// Graficar Newton
// graficoInterpolacion(PlinealN, PcubicaN)

// Graficar LaGrange 
// graficoInterpolacion(Plineal, Pcubica);


function err = errorReal(P, f, x)
    err = abs(horner(P, x) - f(x));
endfunction
/* 
ERRORES DE LAGRANGE:
err_lineal = errorReal(Plineal, exp, 1/3)
err_cubica = errorReal(Pcubica, exp, 1/3) 
*/

// ERRORES DE NEWTON
/*
err_linealNewton = errorReal(PlinealN, exp, 1/3)
err_cubicaNewton = errorReal(PcubicaN, exp, 1/3)
*/

// EJ 5
/*
Tenemos los resultados de f(1), f(2) pero no el de f(3)
Lo podemos encontrar con los datos que nos dieron ya que sabemos que 
P123(2.5) = 3, esto es
L1(2,5)*3 + L2(2,5) * 3 + L3(2,5)*f(3) = 3
f(3) = 3 - (L1(2,5)*3 + L2(2,5) * 3) / L3(2,5)

x=[1,2,3]
L1 = Lk(x,1)
L2 = Lk(x,2)
L3 = Lk(x,3)

// Entonces ahora armemos k
L1y = horner(L1, 2.5) * 3
L2y = horner(L2, 2.5) * 3
L3y = horner(L3, 2.5) // * f(3)
k = (3-L1y-L2y)/L3y
// Ahora podemos armar el polinomio P0123
x = [0,1,2,3]
y = [1,3,3,k]
P0123 = lagrangePoly(x,y)
*/
// printf("El valor de P0123(2.5) es %lf", horner(P0123, 2.5))

// EJ 6
xp = [-1, 1, 2, 4]
a = [2, 1, 2, 2]

x = poly(0,"x")
Pn = 2 + (x+1) - 2*(x+1)*(x-1)+ 2*(x+1)*(x-1)*(x-2)
P30 = horner(Pn, 0)

/* Se sabe que |f^4(x)| <= 33.6 
La formula del error |f(x)-Pn(x)| = (x-x0)(x-x1)(x-x2)(x-x3)/(n+1)! * f^n+1(e)
En este caso
(x+1)(x-1)(x-2)(x-4)/(3+1)! * f^3+1(x)  (3+1)! = 24 y |f^4(x)| <= 33.6 
|f(x)-P3(x)| <= |1*-1*-2*-4/24| * 33.6 = |-8|/24*33.6 = 11.2
 
*/

// EJERCICIO 9 -----------------------------------------------------
function y=nodos(n)
    y = linspace(-5,5,n+1) // Crea nodos espaciados de -5 a 5 (se puede cambiar)
endfunction

function f=funct(x)
    f = 1./(1+x^2)
endfunction

// n = 2, 4, 6, 10, 14
n2 = nodos(2)
n4 = nodos(4)
n6 = nodos(6)
n10 = nodos(10)
n14= nodos(14)

// Imagenes
y1 = funct(n2)
y2 = funct(n4)
y3 = funct(n6)
y4 = funct(n10)
y5 = funct(n14)

P2 = poliNewton(n2,y1)
P4 = poliNewton(n4,y2)
P6 = poliNewton(n6,y3)
P10 = poliNewton(n10,y4)
P14 = poliNewton(n14,y5)

rango = -5:0.1:5
subplot(211)
plot2d(rango, [1./(1.+rango'.^2) horner(P2,rango') horner(P4,rango') horner(P6,rango') horner(P10,rango') horner(P14,rango')], [2,3,4,5,6,1])
a=gca()
a.x_location = "origin"
a.y_location = "origin"
legend("1/x^2+1", "P2", "P4", "P6", "P10", "P16");

function y = errores(f, Pn, x)
    y = abs(f(x) - horner(Pn, x))
endfunction
err1 = errores(funct, P2, rango)
err2 = errores(funct, P4, rango)
err3 = errores(funct, P6, rango)
err4 = errores(funct, P10, rango)
err5 = errores(funct, P14, rango)

subplot(212)
plot2d(rango, [err1', err2', err3', err4', err5'], [3,4,5,6,1])
a=gca()
a.x_location = "origin"
a.y_location = "origin"



