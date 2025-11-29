function a = newtonCoef(x, y)
    n = length(x);
    a = y;
    for k = 2:n
        a(k:n) = (a(k:n) - a(k-1)) ./ (x(k:n) - x(k-1));
    end
endfunction

function w=DDi(x,y)
    n = length(x);
    if n==1 then
        w = y(1)
    else
        w = (DD(x(2:n),y(2:n))-DD(x(1:n-1),y(1:n-1)))/(x(n)-x(1))
    end;
endfunction

function y=poliNewton(x,y)
    n = length(x)
    a = newtonCoef(x,y)
    P = y(1)
    producto = 1;
    for k=2:n
        producto = producto * (poly(0, "x") - x(k-1))
        P = P + a(k) * producto
    end
    y = P
endfunction

xp = [-1 0 1 2 3]
yp = [3 0 -1 1 2]

pol = poliNewton(xp,yp)

xpA = [-1 0 1 2 3 4]
ypA = [3 0 -1 1 2 3]

DD = newtonCoef(xpA, ypA)
equis = poly(0, "x")
n = length(xp)
n4 = 1
for i=1:5
    n4 = n4*(equis-xp(i))
end
pol = pol + DD(6)*n4

// EJ 2
xi = [0 0.25 0.5 0.75]
yi = [2 1 0 1]
// Vandermonde

function y=construirA(x)
    n = length(x)
    A = zeros(n,2)
    A(:, 1) = ones(n)'; A(:, 2) = cos(2*%pi*x(:))
    y= A
endfunction

A = construirA(xi)
[Q,R] = qr(A, "e")
w = inv(R)*Q'*yi'
poli = poly(w, "x", "coeff")
rango = -1:0.001:1
a=gca()
a.x_location = "origin"
a.y_location = "origin"
plot2d(xi, yi', -3)
plot2d(rango, horner(poli, cos(2*%pi*rango')), 6, rect=[-3, -3, 3, 3])

err = abs(A*w-yi')




