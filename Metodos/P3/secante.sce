function y=f(x)
    y = (x^2)/4 - sin(x)
endfunction

function y1=f1(x)
    y1 = 2*sin(x) - x^2
endfunction

function root=secante(x0,x1,fun)
    h = 10^-10
    i = 1
    x2 = x1 - (fun(x1)*((x1-x0)/(fun(x1)-fun(x0))))
    while abs(fun(x2)) > h
        x0 = x1
        x1 = x2
        x2 = x1 - (fun(x1)*((x1-x0)/(fun(x1)-fun(x0))))
        i = i + 1
    end
    root = x2
    disp([x0,x1])
    disp(i)
endfunction

disp(secante(-0.5,0.5,f))
