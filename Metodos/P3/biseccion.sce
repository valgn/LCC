function y=f(x)
    y = 2*sin(x) - x^2
endfunction

function root=biseccion(a,b,fun)
    c = (a+b)/2;
    eps = 1e-5;
    i = 0
    while abs(b-a) > eps && abs(fun(c)) > eps
        c = (a+b)/2;
        if fun(a)*fun(c) < 0 then
            b = c;
        else
            a = c;
        end
        i = i + 1;
    end
    root = c;
    disp(i)
endfunction

//disp(bisecc//ion(1,2,f))

//x = -3:0.01:3;   // vector desde -2 hasta 2 con paso 0.01
//y = f(x);
//
//plot(x,y)
//xgrid()  
//xlabel("x")
//ylabel("f(x)")
//title("Grafico de f(x) = 2*sin(x) - x^2")
//plot(x,0*x,"r--")

function b=h(x)
    b = %e^(-x) - x^4;
endfunction

disp(biseccion(0,1,h))



clf()                   // limpia la ventana gráfica
x = -2:0.001:2;
y = h(x);

plot(x,y,"b-");         // la función
plot(x,0*x,"r--");      // el eje x
xgrid()
