function y = ej3()
    for k = 0:10
        e = 0.1*k
        printf("Iteracion %d con epsilon = %.1f\n",k, e);
        
        A = [1 -1 0; -2 4 -2; 0 -1 1+e];
        disp(A)
        
        carac = poly(A, "x")
        printf("Polinomio caracteristico es: ")
        disp(carac)
        
        raices = roots(carac);
        printf("Las raices del caracteristico son: ")
        disp(raices)
        
        autoval = spec(A)
        printf("Los autovalores de A con eps = %.1f", e)
        disp(autoval)
        printf("\n");
        
        y=0
        
    end
endfunction

