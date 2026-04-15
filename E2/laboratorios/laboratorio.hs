module Lab01 where

import Data.List
import Data.Char

{-
1) Corregir los siguientes programas de modo que sean aceptados por GHCi.
-}

-- a)
mynot b = case b of
    True -> False
    False -> True

-- b)
myin :: [a] -> [a]
myin []          =  error "empty list"
myin [x]         =  []
myin (x:xs)      =  x : myin xs

-- c)
mylength []        =  0
mylength (_:l)     =  1 + mylength l

-- d)
list123 = 1:(2:(3:[]))

-- e)
(++!) :: [a] -> [a] -> [a]
(++!) []  ys = ys
(++!) (x:xs) ys = x : ((++!) xs ys)

-- f)
addToTail x xs = map +x tail xs

-- g)
listmin xs = head (sort xs)

-- h)
smap :: (a->a) -> [a] -> [a]
smap f [] = []
smap f [x] = [f x]
smap f (x:xs) = f x : smap f xs


--2. Definir las siguientes funciones y determinar su tipo:

--a) five, que dado cualquier valor, devuelve 5
five :: a -> Int
five _ = 5

--b) apply, que toma una función y un valor, y devuelve el resultado de
--aplicar la función al valor dado
apply :: (a->b) -> a -> b
apply f x = f x
--c) ident, la función identidad
ident :: a->a
ident x = x
--d) first, que toma un par ordenado, y devuelve su primera componente
first :: (a,b)->a
first (x, _) = x
--e) derive, que aproxima la derivada de una función dada en un punto dado
derive :: Floating a => (a->a) -> a -> a
derive f x = (f(x+h) - f(x)) / h
    where h = 0.00001
--f) sign, la función signo
sign :: (Ord a, Num a) => a -> a
sign x | x > 0 = 1
       | x < 0 = -1
       | otherwise = 0

--g) vabs, la función valor absoluto (usando sign y sin usarla)

--h) pot, que toma un entero y un número, y devuelve el resultado de
--elevar el segundo a la potencia dada por el primero

--i) xor, el operador de disyunción exclusiva

--j) max3, que toma tres números enteros y devuelve el máximo entre llos

--k) swap, que toma un par y devuelve el par con sus componentes invertidas
swap :: (a,b) -> (b,a)
swap (x,y) = (y,x)

{- 
3) Definir una función que determine si un año es bisiesto o no, de
acuerdo a la siguiente definición:

año bisiesto 1. m. El que tiene un día más que el año común, añadido al mes de febrero. Se repite
cada cuatro años, a excepción del último de cada siglo cuyo número de centenas no sea múltiplo
de cuatro. (Diccionario de la Real Academia Espaola, 22ª ed.)

¿Cuál es el tipo de la función definida?
-}
esBisiseto :: (Integral a) => a -> Bool
esBisiseto año | año `mod` 400 == 0 = True
              | año `mod` 100 == 0 = False
              | año `mod` 4 == 0 = True
              | otherwise = False


{-
4)

Defina un operador infijo *$ que implemente la multiplicación de un
vector por un escalar. Representaremos a los vectores mediante listas
de Haskell. Así, dada una lista ns y un número n, el valor ns *$ n
debe ser igual a la lista ns con todos sus elementos multiplicados por
n. Por ejemplo,

[ 2, 3 ] *$ 5 == [ 10 , 15 ].

El operador *$ debe definirse de manera que la siguiente;
expresión sea válida:

-}

-- v = [1, 2, 3] *$ 2 *$ 4

infixl *$
(*$) :: Num a => [a] -> a -> [a]
(*$) xs n = map (*n) xs


-- 5) Definir las siguientes funciones usando listas por comprensión:

-- a) 'divisors', que dado un entero positivo 'x' devuelve la
-- lista de los divisores de 'x' (o la lista vacía si el entero no es positivo)
divisors x = [y | y <- [1..x], x `mod` y == 0]
-- b) 'matches', que dados un entero 'x' y una lista de enteros descarta
-- de la lista los elementos distintos a 'x'
matches x xs = [y | y <- xs, y == x]
-- c) 'cuadrupla', que dado un entero 'n', devuelve todas las cuadruplas
-- '(a,b,c,d)' que satisfacen a^2 + b^2 = c^2 + d^2,
-- donde 0 <= a, b, c, d <= 'n'
cuadrupla n = [(a,b,c,d) | a<-[0..n], b<-[0..n], c<-[0..n], d<-[0..n], a^2 + b^2 == c^2 + d^2]
-- (d) 'unique', que dada una lista 'xs' de enteros, devuelve la lista
-- 'xs' sin elementos repetidos

unique :: [Int] -> [Int]
unique xs = [x | (x,i) <- zip xs [0..], not (elem x (take i xs))]

{- 
6) El producto escalar de dos listas de enteros de igual longitud
es la suma de los productos de los elementos sucesivos (misma
posición) de ambas listas.  Definir una función 'scalarProduct' que
devuelva el producto escalar de dos listas.

Sugerencia: Usar las funciones 'zip' y 'sum'. -}
scalarProduct xs ys = sum [x*y | (x,y) <- zip xs ys]



-- 7) Definir mediante recursión explícita
-- las siguientes funciones y escribir su tipo más general:

-- a) 'suma', que suma todos los elementos de una lista de números
suma :: Num a => [a] -> a
suma [] = 0
suma (x:xs) = x + suma xs

-- b) 'alguno', que devuelve True si algún elemento de una
-- lista de valores booleanos es True, y False en caso
-- contrario
alguno [] = False
alguno (x:xs) = x || alguno xs

-- c) 'todos', que devuelve True si todos los elementos de
-- una lista de valores booleanos son True, y False en caso
-- contrario
todos [] = True
todos (x:xs) = x && todos xs

-- d) 'codes', que dada una lista de caracteres, devuelve la
-- lista de sus ordinales
codes [] = []
codes (x:xs) = ord x : codes xs

-- e) 'restos', que calcula la lista de los restos de la
-- división de los elementos de una lista de números dada por otro
-- número dado

-- f) 'cuadrados', que dada una lista de números, devuelva la
-- lista de sus cuadrados
cuadrados [] = []
cuadrados (x:xs) = x^2 : cuadrados xs

-- g) 'longitudes', que dada una lista de listas, devuelve la
-- lista de sus longitudes

-- h) 'orden', que dada una lista de pares de números, devuelve
-- la lista de aquellos pares en los que la primera componente es
-- menor que el triple de la segunda

-- i) 'pares', que dada una lista de enteros, devuelve la lista
-- de los elementos pares

-- j) 'letras', que dada una lista de caracteres, devuelve la
-- lista de aquellos que son letras (minúsculas o mayúsculas)

-- k) 'masDe', que dada una lista de listas 'xss' y un
-- número 'n', devuelve la lista de aquellas listas de 'xss'
-- con longitud mayor que 'n' -}
masDe [] _ = []
masDe (xs:xss) n = if length xs > n then xs : masDe xss n
                    else masDe xss n
