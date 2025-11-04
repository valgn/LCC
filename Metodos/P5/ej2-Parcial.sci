function [x,cont] = sor(A,b,x0,w,eps)
    n = size(A,1);
    x = x0;
    xk = x;
    suma = 0;
    cont = 0;
    
    for i=1:n
    suma = 0
        for j = 1:n
            if (i<>j)
                suma = suma + A(i,j)*x(j)
            end
        end
    x(i) = (1-w)*x(i)+w/A(i,i)*(b(i)-suma)
    end
    cont = cont+1

    while (abs(norm(x-xk))> eps)
        xk = x
        for i=1:n
            suma = 0
            for j = 1:n
                if (i<>j)
                    suma = suma + A(i,j)*x(j)
                end
            end
            x(i) = (1-w)*x(i)+w/A(i,i)*(b(i)-suma)
        end
     cont = cont+1
    end

endfunction

function p=r_sor(w)
    D = diag(diag(A))
    L = tril(A) - D
    U = triu(A) - D
    n = size(A,1)
    Tw = inv((D+w*L))*((1-w)*D-w*U)

    p = max(abs(spec(Tw)))
endfunction


function Grafico(A)
        Gs = r_sor(1)
        x = [0.1:0.1:3]'
        n = size(x,1)
        y = zeros(n,1)
        for i=1:n
            y(i) = r_sor((x(i)))
        end
        plot(x,y)
        plot(x, Gs)
        gca().x_location = 'origin';
        gca().isoview = "on"
endfunction
