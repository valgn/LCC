// 1. Cargamos los datos
x_datos = [0; 0.25; 0.5; 0.75];
y_datos = [2; 1; 0; 1];

// 2. Armamos la matriz A y el vector Y
// Usamos %pi para Pi. Al multiplicar vector por escalar, Scilab lo hace elemento a elemento
A = [ones(4,1), cos(2 * %pi * x_datos)];
Y = y_datos;

// 3. Resolvemos por Mínimos Cuadrados usando la barra invertida
coefs = (A'*A) \ (A'*Y);

a = coefs(1);
b = coefs(2);

disp("El parámetro a es: " + string(a));
disp("El parámetro b es: " + string(b));

// 4. Graficamos
// Primero los datos empíricos como círculos rojos
plot(x_datos, y_datos, 'ro');

// Preparamos un rango continuo para que la curva del coseno se vea suave
x_rango = linspace(-0.1, 1, 100); 
// Evaluamos nuestra función con los parámetros encontrados
y_rango = a + b * cos(2 * %pi * x_rango);

// Dibujamos la función ajustada encima (línea azul por defecto)
plot(x_rango, y_rango);

// Ponemos la leyenda para que quede prolijo
legend(["Puntos experimentales", "Ajuste: y = 1 + 1*cos(2*pi*x)"]);
