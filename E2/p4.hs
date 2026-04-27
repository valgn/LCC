
data Tree t = Empty | Nodo t (Tree t) (Tree t) deriving Show
data BST a = Hoja | Node (BST a) a (BST a) deriving Show

--- a)
completo :: t -> Int -> Tree t
completo _ 0 = Empty
completo t d = Nodo t x x 
             where x = completo t (d-1)
--- b)
balanceado :: a -> Int -> Tree a
balanceado x 0 = Empty
balanceado a nodos = Nodo a l r
                    where l = balanceado a ((nodos - 1) `div` 2)
                          r = balanceado a ((nodos - 1) - ((nodos-1)`div`2))


--- 2 a)
maximumm :: Ord a => BST a -> a
maximumm (Node l x Hoja) = x
maximumm (Node _ a r) = (maximumm r)

--- Definimos un arbol para probar checkBST
arbol :: BST Int
arbol = Node (Node (Node Hoja 1 Hoja) 3 (Node Hoja 4 Hoja)) 5 (Node (Node Hoja 6 Hoja) 7 (Node Hoja 8 Hoja))
--- Definimos otro arbol para probar join
arbol2 :: BST Int
arbol2 = Node (Node (Node Hoja 0 Hoja) 2 (Node Hoja 3 Hoja)) 4 (Node (Node Hoja 9 Hoja) 10 (Node Hoja 11 Hoja))

--- 2b)
checkBST :: Ord a => BST a -> Bool
checkBST Hoja = True
checkBST (Node Hoja m Hoja) = True
checkBST (Node izq medio Hoja) = let (Node _ m _) = izq in if m <= medio then True else False
checkBST (Node Hoja medio der) = let (Node _ m _) = der in if m > medio then True else False
checkBST (Node izq medio der)  = datoIzq <= medio && datoDer > medio && checkBST izq && checkBST der
                               where (Node _ datoIzq _) = izq
                                     (Node _ datoDer _) = der

mayoresaX :: Ord a => BST a -> a -> BST a
mayoresaX Hoja _ = Hoja
mayoresaX (Node izq medio der) x | medio > x = (Node (mayoresaX izq x) medio der)
                                 | otherwise = mayoresaX der x

menoresaX :: Ord a => BST a -> a -> BST a
menoresaX Hoja _ = Hoja
menoresaX (Node izq medio der) x | medio <= x = (Node izq medio (menoresaX der x))
                                 | otherwise = menoresaX izq x


--- 2c)
splitBST :: Ord a => BST a -> a -> (BST a, BST a)
splitBST tree x = (menores, mayores)
                  where menores = menoresaX tree x
                        mayores = mayoresaX tree x

splitBST2 :: Ord a => BST a -> a -> (BST a, BST a)
splitBST2 Hoja _ = (Hoja, Hoja)
splitBST2 (Node izq medio der) x | medio > x = (z1, Node z2 medio der)
                                 | otherwise = (Node izq medio k1 , k2)
                              where (z1,z2) = splitBST2 izq x
                                    (k1,k2) = splitBST2 der x

join :: Ord a => BST a -> BST a -> BST a
join Hoja Hoja = Hoja
join a Hoja = a
join Hoja b = b
join (Node i1 m1 d1) t2 = (Node (join i1 sizq) m1 (join d1 sder))
                                    where (sizq,sder) = splitBST2 t2 m1


memberAux :: (Ord a, Eq a) => BST a -> a -> a -> Bool
memberAux Hoja valor candidato = valor == candidato
memberAux (Node izq medio der) valor candidato | medio <= valor = memberAux der valor medio
                                               | otherwise = memberAux izq valor candidato


member :: (Ord a, Eq a) => BST a -> a -> Bool
member (Node izq m der) valor = memberAux (Node izq m der) valor m

-----------------------------------------------------------------------
data Color = R | B deriving Show
data RBT a = E | T Color (RBT a) a (RBT a) deriving Show

insert :: Ord a => a -> RBT a -> RBT a
insert x t = makeBlack (ins x t)
            where 
                  ins x E = T R E x E
                  ins x (T c l y r) | x < y = lbalance c (ins x l) y r
                                    | x > y = rbalance c l y (ins x r)
                                    | otherwise = T c l y r
                  makeBlack E = E
                  makeBlack (T _ l x r) = T B l x r

balance :: Color -> RBT a -> a -> RBT a -> RBT a
balance B (T R (T R a x b) y c) z d = T R (T B a x b) y (T B c z d)
balance B (T R a x (T R b y c)) z d = T R (T B a x b) y (T B c z d)
balance B a x (T R (T R b y c) z d) = T R (T B a x b) y (T B c z d)
balance B a x (T R b y (T R c z d)) = T R (T B a x b) y (T B c z d)
balance c l a r = T c l a r

rbalance :: Color -> RBT a -> a -> RBT a -> RBT a
rbalance B a x (T R (T R b y c) z d) = T R (T B a x b) y (T B c z d)
rbalance B a x (T R b y (T R c z d)) = T R (T B a x b) y (T B c z d)
rbalance c l a r = T c l a r

lbalance :: Color -> RBT a -> a -> RBT a -> RBT a
lbalance B (T R (T R a x b) y c) z d = T R (T B a x b) y (T B c z d)
lbalance B (T R a x (T R b y c)) z d = T R (T B a x b) y (T B c z d)
lbalance c l a r = T c l a r

-- data Color = R | B
-- data RBT a = E | T Color (RBT a) a (RBT a)

data T123 a = TEmpty | Node2 (T123 a) a (T123 a) | Node3 (T123 a) (T123 a) a a (T123 a) | Node4 (T123 a) (T123 a) a a a (T123 a) (T123 a) deriving Show

-- Definimos un RBT para probar rbt123
rbt :: RBT Int
rbt = T B (T B (T B E 1 E) 2 (T B E 3 E)) 4 (T B (T R E 5 E) 6 (T R E 7 E))

--- Definimos list2RBT para probar rbt123
list2RBT :: Ord a => [a] -> RBT a
list2RBT [] = E
list2RBT (x:xs) = insert x (list2RBT xs)

rbt123 :: RBT a -> T123 a
rbt123 E = TEmpty
rbt123 (T B (T R a rojo1 c) negro (T R x rojo2 z)) = Node4 (rbt123 a)(rbt123 c) rojo1 negro rojo2 (rbt123 x)(rbt123 z)
rbt123 (T B (T R a rojo c) negro hijoder) = Node3 (rbt123 a)(rbt123 c) rojo negro (rbt123 hijoder)
rbt123 (T B hijoizq negro (T R a rojo c)) = Node3 (rbt123 hijoizq)(rbt123 a) negro rojo (rbt123 c)
rbt123 (T B izq m der) = Node2 (rbt123 izq) m (rbt123 der)

--- HEAP
