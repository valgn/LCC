function y=horner(p,punto)
    n = degree(p);
    b = 0
    for i=n:-1:0
        b = coeff(p,i) + punto*b
    end
    y = b
endfunction

function [px, ppx]=hornerderivada(p, punto)
    n = degree(p)
    px = coeff(p,n)
    ppx = 0
    for i=n-1:-1:0
        ppx = px + punto*ppx
        px = coeff(p,i) + punto*px
    end
        
endfunction


p = poly([0 0 1], "x", "coeff")
x0 = 4
[px, ppx] = hornerderivada(p,x0)
disp(px); disp(ppx);
