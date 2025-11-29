//Ejercicio 4
//Diferencias divididas
function z= DD(x,y)
    // Entrada: x,y = vectores de puntos de interpolación (x,y)
    // Salida: z = diferencias divididas en los vectores x e y
    n=length(x)
    if n==2 then
        z=(y(n)-y(1))/(x(n)-x(1))
    else
        z=(DD(x(2:n),y(2:n))-DD(x(1:n-1),y(1:n-1)))/(x(n)-x(1))
    end
endfunction

// Método de Diferencias Divididas de Newton
function z=intNewton(x,y)
    // Entrada: x,y = vectores de puntos de interpolación (x,y)
    // Salida: z = polinomio de diferencias divididas de Newton
    z=0
    s=poly(0,"x")
    n=length(x)
    for j=n:-1:2
        z= z*(s-x(j-1))+(s-x(j-1))*DD(x(1:j),y(1:j))
    end
    z=z+y(1)
endfunction

// Error de interpolación
function z= err(p, x, cot)
    // Entrada: p = valor real, x = nodos de interpolación, cot = cota de |f^(n))|
    // Salida: z = error de interpolación en x = p
    n=length(x)
    z=cot/(factorial(n))
    for i=1:n do
        z = z*abs(p - x(i))
    end
endfunction
// Resolvamos el ejericio

x=[2.0 2.1 2.2 2.3 2.4 2.5]'
y=[0.2239 0.1666 0.1104 0.0555 0.0025 -0.0484]

p= intNewton(x,y)
disp("El polinomio interpolante de la interpolación de Newton es: ")
disp(p)

z1 = horner(p,2.15)

err1 = err(2.15,x,1) // |j_0^(6)(x)| <= 1

disp("El valor aproximado de J_0(2.15) es: "+string(z1))
disp("con error: "+string(err1)+" < 0.5D-06")


z2 = horner(p,2.35)

err2 = err(2.35,x,1) // |j_0^(6)(x)| <= 1

disp("El valor aproximado de J_0(2.35) es: "+string(z2))
disp("con error: "+string(err2)+" < 0.5D-06")
