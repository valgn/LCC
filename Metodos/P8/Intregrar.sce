// Regla del Trapecio
function w = Trapecio(a,b,f)
    // Entrada: a,b = extremos de integración; f = función de Scilab
    // Salida: w = aproximación de la integral de f en [a,b] por la Regla del Trapecio
    w = ((b-a)/2)*(f(a)+f(b))
endfunction

// Regla de Simpson
function w = Simpson(a,b,f)
    // Entrada: a,b = extremos de integración; f = función de Scilab
    // Salida: w = aproximación de la integral de f en [a,b] por la Regla de Simpson
    h = (b-a)/2
    w = (h/3)*(f(a)+4*f(a+h)+f(b))
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

// EJ 5
function y=IntDobTrap(f, a,b,c,d, n, m)
    hx = (b-a)/n
    Int = 0
    for k=0:n
        xi = a + k*hx
        y1 = c(xi)
        y2 = d(xi)
        
        hy = (y2 - y1)/m
        
        Inty = (f(xi, y1) + f(xi, y2)) / 2
        for j=1:m-1
            yj = y1 + j*hy
            Inty = Inty + f(xi,yj)
        end
        
        if(k == 0 || k == n) then
            Int = Int + 1/2*Inty *hy
        else
            Int = Int + Inty*hy
        end
    end
    Int = Int*hx
    y = Int
endfunction

// HECHO POR PROFE
function v=DobleTn_opt(f,a,b,c,d,n,m)
    h = (b-a)/n
    
    function t = Tn_fijo_x(x)
      function fy = f_fijo(y)
        fy = f(x,y);
      endfunction
      t = Tn(f_fijo,c(x),d(x),m);    
    endfunction
      
    
    v = (Tn_fijo_x(a) + Tn_fijo_x(b)) / 2;
    for i = 1: n-1
        xi = a+i*h;
        v = v + Tn_fijo_x(xi);
    end
    v = h*v;
endfunction


function I=IntDobSimp(f,a,b,c,d,n,m)
    hx = (b-a)/n
    Int = 0
    for k=0:n
        xi = a + k*hx
        y1 = c(xi)
        y2 = d(xi)
        
        hy = (y2-y1)/m
        Inty = f(xi, y1) + f(xi, y2)
        for j=1:n-1
            yj = y1 + j*hy
            if(modulo(j,2) == 0) then
                Inty = Inty + 2*f(xi,yj)
            else
                Inty = Inty + 4*f(xi,yj)
            end
        end
        
        if (k == 0 || k == n) then
            Int = Int + Inty*(hy/3)
        else 
            if (modulo(k,2)==0) then
                Int = Int + 2*Inty*(hy/3)
            else
                Int = Int + 4*Inty*(hy/3)
            end
        end
    end
    Int = Int*(hx/3)
    I = Int
endfunction


function z = f(x,y)
    z = cos(2*x*y)
endfunction

function y=c(x)
    y = 0
endfunction
function y=d(x)
    y = 1
endfunction
function z = f2(x,y)
    z = 1;
endfunction

// EJERCICIO 6
function y = d2(x)
    y = sqrt(2*x-x^2);
endfunction

function y = c2(x)
    y = -sqrt(2*x-x^2);
endfunction

function y= f3(x,y)
    y = %e^(x+y)
endfunction




