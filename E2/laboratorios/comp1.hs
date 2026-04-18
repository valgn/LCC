module Lab1C where

import Data.List
import Data.Ord
import Distribution.Simple.Utils (xargs)

type Texto = String

{-
   Definir una función que dado un caracter y un texto calcule la frecuencia 
   con la que ocurre el caracter en el texto
   Por ejemplo: frecuency 'a' "casa" = 0.5 
-}
strlen :: Texto -> Int -> Int
strlen [] i = i
strlen (x:xs) i = strlen xs (i+1)

cantChar :: Texto -> Char -> Int -> Int
cantChar [] c i = i
cantChar (x:xs) c i | x == c = cantChar xs c (i+1)
                    | otherwise = cantChar xs c i

frecuency :: Char -> Texto -> Float
frecuency c (x:xs) = fromIntegral(cantChar (x:xs) c 0) / fromIntegral(strlen (x:xs) 0) 

{-                      
  Definir una función frecuencyMap que dado un texto calcule la frecuencia 
  con la que ocurre cada caracter del texto en éste.
  La lista resultado debe estar ordenada respecto a la frecuencia con la que ocurre 
  cada caracter, de menor a mayor frecuencia. 
    
  Por ejemplo: frecuencyMap "casa" = [('c',0.25),('s',0.25),('a',0.5)]

-}
-- mapAux :: Texto -> [(Char, Float)] -> [(Char, Float)]
-- mapAux [] x = x
-- mapAux (x:xs) [] = mapAux xs ((x, frecuency x (x:xs)) : [])
-- mapAux (x:xs) (tupla:resto) = if frecuencia_x < snd tupla then mapAux xs ((x, frecuencia_x) : (tupla:resto))  
--                               else mapAux xs (tupla:((x, frecuencia_x):resto))
--                               where frecuencia_x = frecuency x (x:xs)


frecuencyMap :: Texto -> [(Char, Float)]
frecuencyMap texto = sortBy (comparing snd)[(letra, frecuency letra texto) | letra <- nub texto]

{-
  Definir una función subconjuntos, que dada una lista xs devuelva una lista 
  con las listas que pueden generarse con los elementos de xs.

  Por ejemplo: subconjuntos [2,3,4] = [[2,3,4],[2,3],[2,4],[2],[3,4],[3],[4],[]]
-}
-- [2,3,4] -> 
subconjuntos :: [a] -> [[a]]
subconjuntos [] = [[]]
subconjuntos (x:xs) =  let lista = subconjuntos xs in [x : sublistas | sublistas <- lista] ++ lista

{-
 Definir una función intercala :: a -> [a] -> [[a]]
 tal que (intercala x ys) contiene las listas que se obtienen
 intercalando x entre los elementos de ys. 
 
 Por ejemplo: intercala 1 [2,3]  ==  [[1,2,3],[2,1,3],[2,3,1]]
-}


intercala :: a -> [a] -> [[a]]
intercala num lista =  [take i lista ++ [num] ++ drop i lista | i <- [0 .. (length lista)]]

{- 
  Definir una función permutaciones que dada una lista calcule todas las permutaciones
  posibles de sus elementos. Ayuda: la función anterior puede ser útil. 

  Por ejemplo: permutaciones "abc" = ["abc","bac","bca","acb","cab","cba"]
-}                  

permutaciones :: [a] -> [[a]]
permutaciones [x] = [[x]]
permutaciones (x:xs) = [k | perm <- permutaciones xs, k <- intercala x perm]
