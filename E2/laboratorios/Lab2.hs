
module Lab02 where

{-
   Laboratorio 2
   EDyAII 2022
-}

import Data.List
import Distribution.Simple.Utils (xargs)

-- 1) Dada la siguiente definición para representar árboles binarios:

data BTree a = E | Leaf a | Node (BTree a) (BTree a)

-- Definir las siguientes funciones:

-- a) altura, devuelve la altura de un árbol binario.

t1 :: BTree Int
t1 = Node (Node (Leaf 1) (Leaf 2)) (Leaf 3)
arbol :: BTree Integer
arbol = Node (Node(Leaf 2)(Leaf 3)) (Node(Leaf 4)(Leaf 5))

altura :: BTree a -> Int
altura E = 0
altura (Leaf a) = 0
altura (Node a b) = 1 +  max (altura a) (altura b)

-- b) perfecto, determina si un árbol binario es perfecto (un árbol binario es perfecto si cada nodo tiene 0 o 2 hijos
-- y todas las hojas están a la misma distancia desde la raı́z).

perfecto :: BTree a -> Bool
perfecto E = True
perfecto (Leaf a) = True
perfecto (Node a b) = altura a == altura b && perfecto a && perfecto b

-- c) inorder, dado un árbol binario, construye una lista con el recorrido inorder del mismo.

inorder :: BTree a -> [a]
inorder E = []
inorder (Leaf a) = [a]
inorder (Node a b) = inorder(a) ++ inorder(b)


-- 2) Dada las siguientes representaciones de árboles generales y de árboles binarios (con información en los nodos):

data GTree a = EG | NodeG a [GTree a] deriving Show

data BinTree a = EB | NodeB (BinTree a) a (BinTree a) deriving Show



{- Definir una función g2bt que dado un árbol nos devuelva un árbol binario de la siguiente manera:
   la función g2bt reemplaza cada nodo n del árbol general (NodeG) por un nodo n' del árbol binario (NodeB ), donde
   el hijo izquierdo de n' representa el hijo más izquierdo de n, y el hijo derecho de n' representa al hermano derecho
   de n, si existiese (observar que de esta forma, el hijo derecho de la raı́z es siempre vacı́o).
   
   
   Por ejemplo, sea t: 
       
                    A 
                 / | | \
                B  C D  E
               /|\     / \
              F G H   I   J
             /\       |
            K  L      M    
   
   g2bt t =
         
                  A
                 / 
                B 
               / \
              F   C 
             / \   \
            K   G   D
             \   \   \
              L   H   E
                     /
                    I
                   / \
                  M   J  
-}

garbol = NodeG 10 [
         NodeG 5 [],
         NodeG 15 [NodeG 5 [], NodeG 17 []],
         NodeG 20 []
   ] 


g2bt :: GTree a -> BinTree a
g2bt EG = EB
g2bt (NodeG x hijos) = NodeB (listaABin hijos) x EB

listaABin :: [GTree a] -> BinTree a
listaABin [] = EB
listaABin (EG : hermanos) = listaABin hermanos
listaABin (NodeG x hijos : hermanos) = NodeB (listaABin hijos) x (listaABin hermanos)


inorderBin :: BinTree a -> [a]
inorderBin EB = []
inorderBin (NodeB a b c) = inorderBin a ++ [b] ++ inorderBin c


-- 3) Utilizando el tipo de árboles binarios definido en el ejercicio anterior, definir las siguientes funciones: 
{-
   a) dcn, que dado un árbol devuelva la lista de los elementos que se encuentran en el nivel más profundo 
      que contenga la máxima cantidad de elementos posibles. Por ejemplo, sea t:
            1
          /   \
         2     3
          \   / \
           4 5   6
                             
      dcn t = [2, 3], ya que en el primer nivel hay un elemento, en el segundo 2 siendo este número la máxima
      cantidad de elementos posibles para este nivel y en el nivel tercer hay 3 elementos siendo la cantidad máxima 4.
   -}

-- data BinTree a = EB | NodeB (BinTree a) a (BinTree a) deriving Show

-- definimos un arbol para testear la función dcn
binTree :: BinTree Int
binTree = NodeB (NodeB EB 2 EB) 1 (NodeB EB 3 (NodeB EB 4 EB)) 
-- definimos un arbol con 5 niveles para testear la función dcn
binTree2 :: BinTree Int
binTree2 = NodeB (NodeB (NodeB EB 5 EB) 2 (NodeB EB 6 EB)) 1 (NodeB (NodeB EB 7 EB) 3 (NodeB EB 4 (NodeB EB 6 EB)))

findDepth :: BinTree a -> Int -> Int 
findDepth EB i = i
findDepth (NodeB EB a _) i = i
findDepth (NodeB _ a EB) i = i
findDepth (NodeB lc a rc) i = findDepth lc (i + 1) `max` findDepth rc (i + 1)

aux :: BinTree a -> Int -> Int -> [a]
aux EB _ _ = []
aux (NodeB lc a rc) idx tope = if idx == tope then [a]
                                 else aux lc (idx + 1) tope ++ aux rc (idx + 1) tope

dcn :: BinTree a -> [a]
dcn EB = []
dcn (NodeB lc a rc) = aux (NodeB lc a rc) 0 i
                        where i = findDepth (NodeB lc a rc) 0

{- b) maxn, que dado un árbol devuelva la profundidad del nivel completo
      más profundo. Por ejemplo, maxn t = 2   -}

maxn :: BinTree a -> Int
maxn = undefined

{- c) podar, que elimine todas las ramas necesarias para transformar
      el árbol en un árbol completo con la máxima altura posible. 
      Por ejemplo,
         podar t = NodeB (NodeB EB 2 EB) 1 (NodeB EB 3 EB)
-}

podar :: BinTree a -> BinTree a
podar = undefined






