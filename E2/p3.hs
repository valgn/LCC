import Data.Binary.Get (Decoder(Fail))
import Data.Functor.Classes (Eq1)
import GHC.Exts.Heap (GenClosure(ConstrClosure))
import Distribution.Simple.Setup (emptyCleanFlags)
import Distribution.FieldGrammar (List)
import GHC.Base (VecElem(Int16ElemRep))
import Data.Char (digitToInt)

data Color = C{red :: Float, green :: Float, blue :: Float} deriving Show

color1  = C{red = 255, green = 8, blue = 8}
color2 = C{red = 242, green = 232, blue = 0}

mezclar:: Color -> Color -> Color 
mezclar a b = C{red = (red a + red b) / 2, green = (green a + green b) / 2, blue = (blue a + blue b) / 2} 

-- Ejercicio 2
data Values = Letra Char | Cursor deriving (Show, Eq)
type Linea = [Values]

linea :: Linea
linea = [Letra 'h',Cursor] -- aCloh

vacia :: Linea -> Bool
vacia [] = True
vacia a = False
------------------------------

auxMover :: Linea -> Linea
auxMover (x:lista) = if x == Cursor then y:[x] ++ tail lista
                else x:auxMover lista
                    where y = head lista 

moverIzq :: Linea -> Linea
moverIzq [] = []
moverIzq (Cursor:xs) = Cursor:xs
moverIzq xs =  reverse(auxMover (reverse xs))

moverDer :: Linea -> Linea
moverDer [] = []
moverDer xs = if head (reverse xs) == Cursor then xs
              else auxMover xs

--------------------------------

auxIni :: Linea -> Linea -> (Linea, Linea)
auxIni (x:xs) prim = if x == Cursor then (x:reverse prim, xs)
                     else auxIni xs (x:prim)

moverIni :: Linea -> Linea
moverIni [] = []
moverIni (Cursor:xs) = Cursor:xs
moverIni xs = (fst x) ++ snd x
                where x = auxIni xs []

moverFin :: Linea -> Linea
moverFin [] = []
moverFin xs = if head(reverse xs) == Cursor then xs
              else tail x ++ [head x]
              where x = moverIni xs
----------------------

auxIns :: Linea -> Linea -> Char -> Linea
auxIns (x:xs) prim c = if x == Cursor then prim ++ Letra c:[x] ++ xs
                       else auxIns xs (prim ++ [x]) c

insertar :: Linea -> Char -> Linea
insertar [] _ = []
insertar xs c = auxIns xs [] c

auxBor :: Linea -> Linea -> Linea
auxBor (x1:(x2:xs)) prim = if x2 == Cursor then prim ++ x2:xs
                       else auxBor (x2:xs) (prim ++ [x1])
                       

borrar :: Linea -> Linea
borrar [] = []
borrar (Cursor:xs) = Cursor:xs
borrar xs = auxBor xs []
-----------------------------------------------------------------------
data CList a = EmptyCL | CUnit a | Consnoc a (CList a) a deriving Show

clista = Consnoc 1 (Consnoc 2 EmptyCL  3) 4

cvacia = EmptyCL

cuno = CUnit 3

headCL :: CList a -> a
headCL (CUnit a) = a
headCL (Consnoc a b c) = a

tailCL :: CList a -> CList a
tailCL (CUnit a) = EmptyCL
tailCL (Consnoc a EmptyCL c) = CUnit c
tailCL (Consnoc a b c) = Consnoc (headCL b) (tailCL b) c

isEmptyCL :: CList a -> Bool
isEmptyCL EmptyCL = True
isEmptyCL _ = False

isCUnit :: CList a -> Bool
isCUnit (CUnit a) = True
isCUnit _ = False

imprimirLista :: CList a -> [a]
imprimirLista EmptyCL = []
imprimirLista (CUnit a) = [a]
imprimirLista (Consnoc a b c) = a : imprimirLista b ++ [c]

-- b)
reverseCL :: CList a -> CList a
reverseCL EmptyCL = EmptyCL
reverseCL (CUnit a) = CUnit a
reverseCL (Consnoc a EmptyCL c) = Consnoc c EmptyCL a
reverseCL (Consnoc a b c) = Consnoc c (reverseCL b) a

-- c
listados = Consnoc 1 (Consnoc 2 (Consnoc 3 EmptyCL 4) 5) 6

--[1,2,3,4] -- [[], inits [2,3] , [1,2,3,4]] -> [[], [], [2,3], [1,2,3,4]] ->[[], [], [], [2,3], [1,2,3,4]]

consCL :: a -> CList a -> CList a
consCL x EmptyCL = CUnit x
consCL x (CUnit a) = Consnoc x EmptyCL a
consCL x (Consnoc a b c) = Consnoc x (consCL a b) c

colorado :: a -> CList(CList a) -> CList(CList a)
colorado x EmptyCL = EmptyCL
colorado x (CUnit lista) = CUnit (consCL x lista)
colorado x (Consnoc fstList mdmList lastList) = Consnoc (consCL x fstList) (colorado x mdmList) (consCL x lastList)

inits :: CList a -> CList (CList a)
inits EmptyCL = CUnit EmptyCL
inits (CUnit a) = Consnoc EmptyCL EmptyCL (CUnit a)
inits (Consnoc a b c) = Consnoc EmptyCL (colorado a(inits b)) (Consnoc a b c)

-- d Definir una funcion lasts que toma una CList y devuelve una CList con todas las posibles terminaciones de la CList.

snocCL :: a -> CList a -> CList a
snocCL x EmptyCL = CUnit x
snocCL x (CUnit a) = Consnoc a EmptyCL x
snocCL x (Consnoc a b c) = Consnoc a (snocCL c b) x

mapsnocCL :: a -> CList(CList a) -> CList(CList a)
mapsnocCL x EmptyCL = EmptyCL
mapsnocCL x (CUnit lista) = CUnit (snocCL x lista)
mapsnocCL x (Consnoc fstList mdmList lastList) = Consnoc (snocCL x fstList) (mapsnocCL x mdmList) (snocCL x lastList)

lasts :: CList a -> CList (CList a)
lasts EmptyCL = CUnit EmptyCL
lasts (CUnit a) = Consnoc (CUnit a) EmptyCL EmptyCL
lasts (Consnoc a b c) = Consnoc (Consnoc a b c) (mapsnocCL c (lasts b)) (EmptyCL)

-- e
-- Creamos dos CList de CLists para probar concatCL y concatenarListas
clistaDeListas :: CList (CList Int)
clistaDeListas = Consnoc (CUnit 1) (Consnoc (CUnit 2) (CUnit (CUnit 3)) (CUnit 4)) (CUnit 5)

clistaDeListas2 :: CList (CList Int)
clistaDeListas2 = Consnoc (CUnit 6) (Consnoc (CUnit 7) (CUnit (CUnit 8)) (CUnit 9)) (CUnit 10)


concatenarListas :: CList a -> CList a -> CList a
concatenarListas EmptyCL lista = lista
concatenarListas lista EmptyCL = lista
concatenarListas (CUnit a) lista = consCL a lista
concatenarListas lista (CUnit a) = snocCL a lista
concatenarListas (Consnoc a1 b1 c1) (Consnoc a2 b2 c3) = 
    Consnoc a1 (concatenarListas (snocCL c1 b1)(consCL a2 b2)) c3

concatCL :: CList(CList a) -> CList a
concatCL EmptyCL = EmptyCL
concatCL (CUnit lista) = lista
concatCL (Consnoc fstList mdmList lstList) = concatenarListas (concatenarListas fstList (concatCL mdmList)) lstList

------------------------------------------------------------------
data Exp = Lit Int | Add Exp Exp | Sub Exp Exp | Prod Exp Exp | Div Exp Exp deriving (Show, Eq)
eval :: Exp -> Int
eval (Lit a) = a
eval (Add a b) = eval a + eval b
eval (Sub a b) = eval a - eval b
eval (Prod a b) = eval a * eval b
eval (Div a b) = eval a `div` eval b

-- Ir apilando y si el que ingresa es un operador, operar los ultimos 2 elementos
-- Idea de pila necesitamos una lista de Exp

eliminarEspacios :: String -> String
eliminarEspacios [] = []
eliminarEspacios (x:xs) | x == ' ' = eliminarEspacios xs
                        | otherwise = x : eliminarEspacios xs


-- 8+9 -> 8
aux :: [Char] -> [Exp] -> Exp
aux [] [res] = res
aux (x:xs) (n1:n2:op) | x == '+' = aux xs (Add n2 n1 : op)
                      | x == '-' = aux xs (Sub n2 n1 : op)
                      | x == '*' = aux xs (Prod n2 n1 : op)
                      | x == '/' = aux xs (Div n2 n1 : op)
aux (x:xs) e = aux xs (Lit (digitToInt x) : e)

a =  "8 5 3 - 3 * +"  
b =  "8 5 +"      

parseRPN :: [Char] -> Exp
parseRPN s = aux (eliminarEspacios s) []

--b 
evalRPN :: String -> Int
evalRPN string = eval (parseRPN string)