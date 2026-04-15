aplicar3 :: (Int -> Int) -> Int
aplicar3 f = f 3 

f2 :: Int -> (Int -> Int)
f2 x = (+x)

f3 :: (Int -> Int) -> (Int -> Int)
f3 f x = f (x+1)

f4 :: Int -> Bool
f4 x = x > 0

f5 :: Bool -> (Bool -> Bool)
f5 x y = x && y

ej4 = if true then false
            else true 
            where   false = True;
                    true = False

zip3Rec :: [a] -> [b] -> [c] -> [(a,b,c)]
zip3Rec [] [] [] = []
zip3Rec (x:xs) (y:ys) (z:zs) = (x,y,z) : zip3Rec xs ys zs

zip3zip :: [a] -> [b] -> [c] -> [(a,b,c)]
zip3zip x y z = [(a,b,c) | ((a,b),c) <- zip(zip x y)z]


modulus xs = sqrt (sum (map (^2) xs))