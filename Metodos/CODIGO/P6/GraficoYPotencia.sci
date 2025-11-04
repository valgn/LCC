function circ(r, x, y)
    plot2d(0,0,rect=[-10,-10,10,10],frameflag=3,axesflag=4);
    xarc(x-r, y+r, 2*r, 2*r,0, 360*64)
    gca().x_location = 'origin';
    gca().isoview = "on"
endfunction

function Gers(A)
    clf();
    xgrid();
    n = size(A,1)
    for i=1:n
        circ(sum(abs(A(i,:)))-abs(A(i,i)), A(i,i), 0) 
    end

endfunction

function CircGersValor(A)
    autoval = spec(A);
    disp(autoval)
    Gers(A)
    plot2d(real(autoval), imag(autoval), style=-5)
endfunction

function [l,v]=potmethod(A)
    
    [n,m] = size(A)
    z = zeros(m,1) + 1 // z0
    w = A*z     // w1
    zk = z
    z = w/norm(w,%inf)
    while (norm(z - zk) > 10^-7)
        zk = z
        w = A*z 
        z = w/norm(w,%inf)
    end
    [maxv, j] = max(abs(z))
    l = w(j)/zk(j)
    v = z

    
endfunction

A = [10 1 0 0; 3 0 1 2; 1 1 -3 1; 1 0 1 -4];
CircGersValor(A)



