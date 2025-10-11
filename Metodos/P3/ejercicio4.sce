function y = funciones(k)
    y(1) = 1+k(1)^2-k(2)^2+exp(k(1))*cos(k(2))
    y(2) = 2*k(1)*k(2)+exp(k(1))*sin(k(2))
endfunction

function nm = newtonMultivariable(f,x0,tol,iter,h)
    
    J = numderivative(f,x0,h);
    x1 = x0 - (inv(J)*f(x0))'; // *' = transpuesta *
    i = 1;
    delta = norm(x1-x0);
    
    while (delta>tol) && (i < iter)
        x0 = x1;
        J = numderivative(f,x0,h);
        x1 = x0 - (inv(J)*f(x0))';
        delta = norm(x1-x0);
        i = i + 1;
    end
    
    nm = x1;
    printf("Iteraciones: %d\n", i);

endfunction

vectorInicial = [-1,4];
errorDeseado = 0.0001;
maxIteraciones = 1000;
solucion = newtonMultivariable(funciones, vectorInicial, errorDeseado, maxIteraciones,2);
disp(solucion);
