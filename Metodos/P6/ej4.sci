function circ(r, x, y)
    plot2d(0,0,rect=[-10,-10,10,10],frameflag=3,axesflag=4);
    xarc(x-r, y+r, 2*r, 2*r,90*64, 270*64)
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

A = [4 -1 0; -1 4 -1; -1 -1 4];
