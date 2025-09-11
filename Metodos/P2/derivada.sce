function v = derivar(f,vx,n,h)
    if n==0
         v=f(vx)
    else 
         v=(derivar(f,vx+h,n-1,h)-derivar(f,vx,n-1,h))/h
    end
endfunction

function y=f(x)
    y = sin(x)
endfunction

disp(derivar(f, %pi, 2 ,10^-3))

