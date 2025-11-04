function gershgorin_plot(A)
    n = size(A, 1);
    clf(); // limpiar ventana de gráficos
    theta = 0:0.01:2*%pi;
    
    for i = 1:n
        centro = A(i, i);
        radio = sum(abs(A(i, :))) - abs(A(i, i));
        
        // Coordenadas del círculo
        x = real(centro) + radio * cos(theta);
        y = imag(centro) + radio * sin(theta);
        
        // Graficar el círculo
        plot(x, y, 'b-');
        // Marcar el centro
        plot(real(centro), imag(centro), 'ro');
    end
    
    // Hacer los ejes iguales y poner cuadrícula
    a = gca();
    a.isoview = "on";
    xgrid();
    xlabel("Parte real");
    ylabel("Parte imaginaria");
    title("Círculos de Gershgorin");
endfunction

