function t=diagonalDominante(A)
    diagDom = %T
    for i=1:size(A,1)
        if(abs(A(i,i)) <= sum(abs(A(i, :))) - abs(A(i,i)))
            diagDom = %F
            break
        end
    end
    t = diagDom    
endfunction

function r=definidaPositiva(A)
    r = %T
    autoval = spec(A)
    for i=1:size(autoval, 1)
        if(real(autoval(i)) <= 0)
            r = %F
            break
        end
    end
    
endfunction

function r=esSimetrica(A)
    if(A == A') then
        r = %T
    else 
        r = %F
    end
endfunction

function graficoConvergencia(errores)
    plot(1:length(errores), errores)
    xlabel("Iteraciones")
    ylabel("Error relativo")
    title("Convergencia del método")
endfunction
