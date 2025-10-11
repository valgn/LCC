function y=triangular(A,b)
    [n,m] = size(A)
    x = zeros(n,1)
    x(n) = b(n)/A(n,m)
    for i=n-1:-1:1
        x(i) = (b(i) - A(i,i+1:n)*x(i+1:n))/A(i,i)
    end
    y=x
endfunction


function y=inferior(A,b)
    [n,m] = size(A)
    x=zeros(n,1)
    x(1) = b(1)/A(1,1)
    for i=2:1:n
        x(i) = (b(i) - A(i, 1:i)*x(1:i))/A(i,i)
    end
    y=x
endfunction
    

A=[1,2,3;0,1,4;0,0,5;]
b= [1,8,6]'

A2=[1,0,0;2,3,0;1,2,1;]

disp(triangular(A,b))
disp(inferior(A2,b))

