function v = derivar(f,vx,n,h)
    if n==0
         v=f(vx)
    else 
         v=(derivar(f,vx+h,n-1,h)-derivar(f,vx,n-1,h))/h
    end
endfunction

function y= taylor(f, a, v, n)
    y =0
    for k = 0:n
        //dk_fun = numderivative(f, a,[], order=4)
        //dk = dk_fun(a)  
        dk = derivar(f, v, k, 10^-5)               
        y = y + (dk / factorial(k)) * (v - a)^k;
    end
endfunction

function y=fu(x)
    y = exp(x)
endfunction

disp(taylor(fu, 1.19, 1.2, 3))
disp(exp(1.2))

//La funcion derivar tiene mucho error en ordenes grandes
//LA funcion numderivative solo va hasta orden 4
