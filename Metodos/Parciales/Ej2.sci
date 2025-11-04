function a=ItJacob(A)
    [n,m] = size(A)
    I = eye(n, m)
    N = diag(diag(A))
    Ninv = inv(N)
    a = I-Ninv*A
endfunction

A = [2 1; 1 2]
