// 2 a
function p = r_sor(w, A)
    D = diag(diag(A))
    L = tril(A) - D
    U = triu(A) - D
    [n, m] = size(A)
    I = eye(n, n)
    Tw = inv(D + w*L)*((1-w)*D - w*U)
    M = I - inv(D/w + L)*A

    p = max(abs(spec(M)))
endfunction

// 2 b
function grafico(A)
    Gs = r_sor(1)
    x = [0.1 : 0.1 : 3] // Arma un vector con saltos de 0.1
                        // desde 0.1 hasta 3
    [n,m] = size(x)
    
    y = zeros(1, m)
    y2 = y
    
    for i = 1: m
        y2(i) = r_sor(1)
        y(i) = r_sor(x(i))
    end
    
    scf()
    plot(x, y, "r-+", x, y2, "b-")
endfunction

// 2 c
function y = w_opt(A)
    autov = spec(A)
    for i = 1 : size(autov, 1)
        if autov(i) <= 0
            printf("La matriz no es definida positiva\n")
            break
        end
    end
    n = size(A, 1)
    I = eye(n, n)
    D = diag(diag(A))
    radEspect = max(abs(spec(I - inv(D)*A)))
    y = 2/(1 + sqrt(1 - radEspect^2))
endfunction

// 2 d
function y = GS(A, b, x0, e)
    x = x0
    N = tril(A) 
    Ninversa = inv(N)
    xk = Ninversa*((N-A)*x0 + b)
    count = 1
    
    while abs(norm(A*x-b)) > e
        x = Ninversa*((N-A)*xk + b)
        xk = x
        count = count + 1
    end
    disp(count)
    y = x
endfunction

// 2 e 
function y = relaja(A, b, x0, e, w)
    x = x0
    D = diag(diag(A))
    N = D/w + tril(A) - D
    Ninversa = inv(N)
    xk = Ninversa*((N-A)*x0 + b)
    count = 1
    
    while abs(norm(A*x-b)) > e
        x = Ninversa*((N-A)*xk + b)
        xk = x
        count = count + 1
    end
    disp(count)
    y = x
endfunction

//grafico()

//disp(w_opt())

/*
A = [10 5 0 0; 5 10 -4 0; 0 -4 8 -1; 0 0 -1 5]
x0 = zeros(4, 1)
b = [6 25 -11 -11]'
e = 1e-6
disp(relaja(A, b, x0, e, 1.15))
*/
