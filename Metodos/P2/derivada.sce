function y=derivar(f,v,n,h)
    for i=1:1:n
        deff("res=tmp(x)", "res=(f(v+h) - f(v))/h")
        y=tmp
    end
    y =y(v)
endfunction

function y=f(x)
    y = sin(x)
endfunction

disp(derivar(f, %pi, 2, 10^-8))

