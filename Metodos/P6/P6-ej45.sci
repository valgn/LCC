function circ(r, x, y)
    xarc(real(x)-r, imag(y)+r, 2*r, 2*r, 0, 360*64);
endfunction

function Gers(A)
    clf();
    n = size(A,1);
    
    radios = zeros(n,1);
    for i=1:n
        radios(i) = sum(abs(A(i,:))) - abs(A(i,i));
    end
    
    centros = diag(A);
    xmin = min(real(centros)- radios);
    xmax = max(real(centros) +radios);
    ymin = min(imag(centros) -radios);
    ymax = max(imag(centros) +radios);

    margen = 0.2* max([xmax-xmin,ymax-ymin]);
    xmin = xmin -margen;
    xmax = xmax + margen;
    ymin = ymin - margen;
    ymax = ymax + margen;

    plot2d(0, 0, rect=[xmin,ymin,xmax,ymax], frameflag=3, axesflag=4);
    gca().x_location = "origin";
    gca().y_location = "origin";
    gca().isoview = "on";
    xgrid();


    for i=1:n
        circ(radios(i), A(i,i), 0);
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



