
type Rank = Int
data Heap a = E | N Rank a (Heap a) (Heap a) deriving Show

rank :: Heap a -> Rank
rank E = 0
rank (N r _ _ _) = r

makeH :: a -> Heap a -> Heap a -> Heap a
makeH x a b = if rank a >= rank b then N (rank b + 1) x a b
              else N (rank a + 1) x b a


merge :: Ord a => Heap a -> Heap a -> Heap a
merge h1 E = h1
merge E h2 = h2
merge h1@(N _ x a1 b1) h2@(N _ y a2 b2) =
      if x <= y then makeH x a1 (merge b1 h2)
      else makeH y a2 (merge h1 b2)

fromList :: (Ord a) => [a] -> Heap a
fromList [] = E
fromList (x:xs) = (merge (N 1 x E E) (fromList xs))

data PHeaps a = Empty | Root a [PHeaps a] deriving Show

arbol :: PHeaps Int
arbol = Root 1 [Root 2 [Root 3 [Root 0 []] , Root 9 []] , Root 4 [Root 6 []]]

isLower :: Ord a => a -> PHeaps a -> Bool
isLower _ Empty = True
isLower a (Root x _) = a <= x

isPHeap :: Ord a => PHeaps a -> Bool
isPHeap Empty = True
isPHeap (Root a []) = True
isPHeap (Root a (x:xs)) = (isLower a x) && isPHeap (Root a xs) && (isPHeap x) 
-- isPHeap (Root a hijos) = all (isLower a) hijos && all isPHeap hijos

mergep :: Ord a => PHeaps a -> PHeaps a -> PHeaps a
mergep Empty a = a
mergep a Empty = a
mergep t1@(Root a arboles) t2@(Root b arboles2) | a <= b    = (Root a (t2:arboles))
                                                | otherwise = (Root b (t1:arboles2))

insert :: Ord a => PHeaps a -> a -> PHeaps a
insert arbol x = mergep (Root x []) arbol

concatHeaps :: Ord a => [PHeaps a] -> PHeaps a
concatHeaps [] = Empty
concatHeaps (x:xs) = mergep x (concatHeaps xs)


{-

concatHeaps :: Ord a => [PHeaps a] -> PHeaps a
concatHeaps [] = Empty
concatHeaps [h] = h
concatHeaps (h1:h2:hs) = mergep (mergep h1 h2) (concatHeaps hs)

-}

delMin :: Ord a => PHeaps a -> Maybe (a, PHeaps a)
delMin Empty = Nothing
delMin (Root a lista) = Just (a, concatHeaps lista) 