1. Un arreglo maze de 10×10 de ceros y unos representa un laberinto en el que un viajero debe
    encontrar un camino desde maze[0][0] hasta maze[9][9]. El viajero puede moverse de un cuadrado a
    otro adyacente en la misma columna o fila, pero no puede saltar ningún cuadrado ni moverse en diagonal.
    Además, no puede moverse a ningún cuadrado que contenga un 1 (se consideran paredes). Asumimos que
    maze[0][0] y maze[9][9] contienen 0. Escriba una función que reciba como argumento tal laberinto
    (maze) e imprima una lista de posiciones representando un camino desde maze[0][0] hasta maze[9][9]
    o un mensaje indicando que no existe camino posible a través del laberinto.

                I 0 0 0 0 0 0 0 0 0
                0 0 0 1 0 0 0 0 0 1
                0 0 0 1 0 1 1 0 0 0
                1 1 1 1 0 0 1 0 0 0
                0 0 0 1 1 1 1 1 1 0
                0 0 0 0 1 0 1 0 0 0
                0 0 0 0 1 0 0 0 0 0
                0 0 0 0 1 0 0 0 0 0
                0 0 0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0 0 F
