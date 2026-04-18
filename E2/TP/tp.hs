import Data.Ord (comparing)
import Data.List

data NdTree p = Node (NdTree p) p (NdTree p) Int | Empty deriving (Eq, Ord, Show)

newtype Punto2d = P2d (Double, Double) deriving (Eq, Show)
newtype Punto3d = P3d (Double, Double, Double) deriving (Eq, Show)

--- Definicion de la clase Punto, cada punto definido debe contar con estas funciones definidas
class Punto p where
  dimension :: p -> Int -- devuelve el número de coordenadas de un punto
  coord :: Int -> p -> Double -- devuelve la coordenada k-ésima de un punto (comenzando de 0)

  -- a) calcula la distancia entre dos puntos p = (x0,...,xn) y q = (y0,...,yn)
  dist :: p -> p -> Double -- calcula la distasncia entre dos puntos
  dist p1 p2 = sqrt (distAux (dimension p1) 0 p1 p2)
    where
      distAux :: Int -> Int -> p -> p  -> Double
      distAux dim i p1 p2 | i == (dim-1)  = (coord i p1 - coord i p2)^2
                          | otherwise = (coord i p1 - coord i p2)^2 + distAux dim (i+1) p1 p2
  
-----------------------------------------------------------------------------

--- Instacia: Puntos 2D
instance Punto Punto2d where
  dimension :: Punto2d -> Int
  dimension _ = 2

  coord :: Int -> Punto2d -> Double
  coord 0 (P2d (x, _)) = x
  coord 1 (P2d (_, y)) = y
-----------------------------------------------------------------------------

--- Instancia: Punto 3d
instance Punto Punto3d where
  dimension :: Punto3d -> Int
  dimension _ = 3
  
  coord :: Int -> Punto3d -> Double
  coord 0 (P3d (x, _, _)) = x
  coord 1 (P3d (_, y, _)) = y
  coord 2 (P3d (_, _, z)) = z
-----------------------------------------------------------------------------



-- Puntos de prueba de dos dimensiones
p1_2d :: Punto2d
p1_2d = P2d (7, 2)
p2_2d :: Punto2d
p2_2d = P2d (7, 4)
p3_2d :: Punto2d
p3_2d = P2d (7, 6)
p4_2d :: Punto2d
p4_2d = P2d (2, 3)  
p5_2d :: Punto2d
p5_2d = P2d (4, 7)
p6_2d :: Punto2d
p6_2d = P2d (8, 1)
--- Lista de puntos de dos dimensiones (Ejemplo del PDF)
listaPuntos::[Punto2d]
listaPuntos = [p1_2d, p2_2d, p3_2d, p4_2d, p5_2d, p6_2d]
-----------------------------------------------------------------------------

-- Puntos de prueba de tres dimensiones
p1_3d :: Punto3d
p1_3d = P3d (2, 4, 6)

p2_3d :: Punto3d
p2_3d = P3d (2, 8, 10)
-----------------------------------------------------------------------------


--- FUNCIÓN PARA OBTENER LA MEDIANA Y LA PRIMERA Y SEGUNDA SUBLISTAS USANDO A LA MEDIANA COMO PUNTO DE INFLEXION
--- Mediante una lista de puntos, obtenemos la mediana y su sublista izquierda y derecha (usando la mediana como inicio y final)
mediana :: Punto p => Int -> [p] -> ([p], p, [p])
mediana eje p = (men, m, may)
              where 
                l = length p `div` 2
                (primeraMitad, m:segundaMitad) = splitAt l p
                valorMedio = coord eje m
                mitades = primeraMitad++segundaMitad
                men = [p | p <- mitades, coord eje p <= valorMedio]
                may = [p | p <- mitades, coord eje p > valorMedio]


-----------------------------------------------------------------------------
--- EJEMPLO CON ESTOS PUNTOS: 
-- [(P2d (7.0,4.0),0),(P2d (7.0,6.0),1),(P2d (7.0,2.0),2),(P2d (2.0,3.0),3),(P2d (4.0,7.0),2),(P2d (8.0,1.0),1)]
-- [(P2d (2.0,3.0),3),(P2d (7.0,2.0),2),(P2d (7.0,6.0),1),(P2d (2.0,10.0),3),(P2d (4.0,7.0),2),(P2d (7.0,4.0),0),(P2d (8.0,1.0),1)]
{-
                (7, 4)              x
              /       \ 
           7, 6       8, 1          y
          /    \
      7, 2    4, 7                  x
      /       
   2, 3                           

 
-}

printTree :: Show p => NdTree p -> IO ()
printTree arbol = putStrLn (unlines (dibujar arbol))

dibujar :: Show p => NdTree p -> [String]
dibujar Empty = ["Empty"]
dibujar (Node izq p der eje) = 
    (show p ++ " [eje " ++ show eje ++ "]") : dibujarSubArboles izq der

dibujarSubArboles :: Show p => NdTree p -> NdTree p -> [String]
dibujarSubArboles Empty Empty = []
dibujarSubArboles izq Empty = 
    let lineasIzq = dibujar izq
    in ("└── (I) " ++ head lineasIzq) : map ("    " ++) (tail lineasIzq)
dibujarSubArboles Empty der = 
    let lineasDer = dibujar der
    in ("└── (D) " ++ head lineasDer) : map ("    " ++) (tail lineasDer)
dibujarSubArboles izq der = 
    let lineasIzq = dibujar izq
        lineasDer = dibujar der
    in ("├── (I) " ++ head lineasIzq) : map ("│   " ++) (tail lineasIzq) ++
       ("└── (D) " ++ head lineasDer) : map ("    " ++) (tail lineasDer)

    

-- data NdTree p = Node (NdTree p)    p   (NdTree p) Int    |    Empty            deriving (Eq, Ord, Show)
--                      ArbolIzq   Punto   ArbolDer  Dim         Vacio


-----------------------------------------------------------------------------
--- FUNCIÓN QUE TOMA UN ARBOL Y EL NIVEL DE LA RAIZ Y NOS DEVUELVE UNA LISTA DE TUPLAS CON EL DATO DEL ARBOL Y SU ALTURA
--- La funcion sirve para poder visualizar de forma clara la forma del arbol INORDER
arbolNivelInOrder :: Punto p => NdTree p -> Int -> [(p, Int)]
arbolNivelInOrder Empty _ = []
arbolNivelInOrder (Node a b c lvl) i =  (arbolNivelInOrder a (i+1)) ++  [(b, i)] ++ (arbolNivelInOrder c (i+1))

--- FUNCIÓN QUE TOMA UN ARBOL Y EL NIVEL DE LA RAIZ Y NOS DEVUELVE UNA LISTA DE TUPLAS CON EL DATO DEL ARBOL Y SU ALTURA
--- La funcion sirve para poder visualizar de forma clara la forma del arbol PREORDER
arbolNivelPreOrder:: Punto p => NdTree p -> Int -> [(p, Int)]
arbolNivelPreOrder Empty _ = []
arbolNivelPreOrder (Node a b c lvl) i =  [(b, i)] ++ (arbolNivelPreOrder a (i+1)) ++ (arbolNivelPreOrder c (i+1))

-----------------------------------------------------------------------------
--- FUNCIÓN PRIVADA PARA PASAR DE LISTA DE PUNTOS A UN NDTREE.
--- Toma una lista de puntos el nivel en el que estamos y la dimension (constante)
fromListAux :: Punto p => [p] -> Int -> Int -> NdTree p
fromListAux [] _ _ = Empty
fromListAux puntos lvl dim = Node (fromListAux izq (lvl+1) dim) mid (fromListAux der (lvl+1) dim) eje
                              where 
                                eje = mod lvl dim
                                spuntos = sortOn (coord eje) puntos
                                (izq, mid, der) = mediana eje spuntos

-----------------------------------------------------------------------------
--- FUNCIÓN PUBLICA PARA PASAR DE LISTA DE PUNTOS A NDTREE
--- Toma una lista de Puntos y devuelve un NDTree
fromList :: Punto p => [p] -> NdTree p
fromList [] = Empty
fromList puntos = fromListAux puntos 0 (dimension (head puntos))
    
-----------------------------------------------------------------------------
--- FUNCIÓN PRIVADA PARA INSERTAR UN PUNTO EN UN NDTREE, TOMA EL PUNTO A INSERTAR, EL NIVEL EN EL QUE ESTAMOS Y EL ARBOL EN EL QUE QUEREMOS INSERTAR
--- Devuelve el nuevo arbol con el punto insertado
insertarAux :: Punto p => p -> Int -> NdTree p -> NdTree p
insertarAux punto lvl Empty = Node Empty punto Empty (mod lvl (dimension punto))
insertarAux punto lvl (Node izq md der eje) | coord eje punto <= coord eje md   = (Node (insertarAux punto (lvl+1) izq) md der eje)
                                            | otherwise                         = (Node izq md (insertarAux punto (lvl+1) der) eje)

--- FUNCIÓN PUBLICA PARA INSERTAR UN PUNTO EN UN NDTREE, TOMA EL PUNTO A INSERTAR Y EL ARBOL EN EL QUE QUEREMOS INSERTAR
--- Devuelve el nuevo arbol con el punto insertado
insertar :: Punto p => p -> NdTree p -> NdTree p
insertar punto arbol = insertarAux punto 0 arbol
-----------------------------------------------------------------------------


--- FUNCIÓN PRIVADA PARA CALCULAR EL MAXIMO ENTRE DOS PUNTOS CON RESPECTO AL EJE DADO
--- Devuelve el mayor punto entre los dos.
maxCordGor :: Punto p => Int -> p -> p -> p
maxCordGor eje p1 p2  | (coord eje p1) < (coord eje p2) = p2
                      | otherwise = p1

--- FUNCIÓN PRIVADA PARA CALCULAR EL MÍNIMO ENTRE DOS PUNTOS CON RESPECTO AL EJE DADO.
--- Devuelve el menor punto entre los dos   
minCordGor :: Punto p => Int -> p -> p -> p
minCordGor eje p1 p2  | (coord eje p1) > (coord eje p2) = p2
                      | otherwise = p1

--- FUNCIÓN PRIVADA QUE BUSCA EL MÁXIMO DEL SUBÁRBOL IZQUIERDO DE UN NODO.
--- Si el nodo es una hoja, el máximo es el punto del mismo.
--- Si el nodo no es una hoja, el máximo se encuentra en el subárbol derecho del mismo, por lo que se busca recursivamente.
--- Si el eje es diferente al eje del nodo, 
buscarMaxIzq :: (Eq p, Punto p) => NdTree p -> Int -> p
buscarMaxIzq (Node Empty md Empty _) _ = md
buscarMaxIzq (Node izq md der eje) ejeGor  
  | ejeGor == eje   = case der of
                        Empty -> md
                        _     -> maxCordGor ejeGor md (buscarMaxIzq der ejeGor)
                        
  | otherwise       = maxCordGor ejeGor md (maxCordGor ejeGor (buscarMaxIzq der ejeGor) (buscarMaxIzq izq ejeGor))

--- FUNCIÓN PRIVADA QUE BUSCA EL MÍNIMO DEL SUBÁRBOL DERECHO DE UN NODO.
--- Si el nodo es una hoja, el mínimo es el punto del mismo.
buscarMinDer :: (Eq p, Punto p) => NdTree p -> Int -> p
buscarMinDer (Node Empty md Empty _) _ = md
buscarMinDer (Node izq md der eje) ejeGor  
  | ejeGor == eje   = case izq of
                        Empty -> md
                        _     -> minCordGor ejeGor md (buscarMinDer izq ejeGor)
  | otherwise       = minCordGor ejeGor md (minCordGor ejeGor (buscarMinDer der ejeGor) (buscarMinDer izq ejeGor))

--- FUNCIÓN PRIVADA QUE SE ENCARGA DE DIVIDIR EL CASO DE ELIMINACIÓN DE UN NODO CON DOS HIJOS EN DOS CASOS, DEPENDIENDO 
--- DE SI EL SUBÁRBOL DERECHO ES EMPTY O NO, YA QUE SI EL SUBÁRBOL DERECHO ES EMPTY, EL SUCESOR SERÁ EL MÁXIMO DEL SUBÁRBOL 
--- IZQUIERDO, Y SI NO LO ES, EL SUCESOR SERÁ EL MÍNIMO DEL SUBÁRBOL DERECHO.
hardCase :: (Eq p, Punto p) => NdTree p -> p -> NdTree p
hardCase (Node izq m Empty eje) punto = let sucesor = buscarMaxIzq izq eje
                                        in (Node (eliminar sucesor izq) sucesor Empty eje)   
                                                                      
hardCase (Node izq m der eje) punto   = let sucesor = buscarMinDer der eje
                                        in (Node izq sucesor (eliminar sucesor der) eje)

eliminar :: (Eq p, Punto p) => p -> NdTree p -> NdTree p
eliminar _ Empty = Empty
eliminar punto (Node Empty m Empty eje)  | punto == m  = Empty
                                         | otherwise   = (Node Empty m Empty eje)
eliminar punto (Node izq m der eje)  
    | punto == m = hardCase (Node izq m der eje) punto
    | coord eje punto <= coord eje m = (Node (eliminar punto izq) m der eje)
    | otherwise = (Node izq m (eliminar punto der) eje)

lista2d :: [Punto2d]
lista2d = [(P2d (1,3)), (P2d (1,2)), (P2d (2,3))]
-----------------------


type Rect = (Punto2d, Punto2d)
-- p1 = (x1, y1)    
-- p2 = (x2, y2)

{-
  (x1, y2) --- (x2, y2)
     |            |
  (x1, y1) --- (x2, y1)
-}
-- Definimos un punto y un rectangulo para probar inRegion y ortogonalSearch
p1 :: Punto2d
p1 = P2d (3, 4)
r1 :: Rect
r1 = (P2d (2, 3), P2d (4, 5))

obtenerMayor :: Rect -> (Double, Double, Double, Double)
obtenerMayor (P2d (x, y), P2d (a, b)) = (min x a, min y b, max x a, max y b)

inRegion :: Punto2d -> Rect -> Bool
inRegion p r  | coord 0 p >= x1 && coord 0 p <= x2 && coord 1 p >= y1 && coord 1 p <= y2 = True
              | otherwise = False
                  where (x1, y1, x2, y2) = obtenerMayor r



ortogonalSearch :: NdTree Punto2d -> Rect -> [Punto2d]
ortogonalSearch puntos rect = undefined







--- Receta de lomo al wellington:
-- Ingredientes:
-- - 1 lomo de res de aproximadamente 1 kg
-- - Sal y pimienta al gusto
-- - 2 cucharadas de mostaza Dijon
-- - 200 g de champiñones picados
-- - 1 paquete de masa de hojaldre
-- - 1 huevo batido (para barnizar)
-- Instrucciones:
-- 1. Precalienta el horno a 200°C (400°F).
-- 2. Sazona el lomo de res con sal y pimienta al gusto
-- 3. En una sartén caliente, sella el lomo por todos lados hasta que esté dorado. Retira del fuego y deja enfriar.
-- 4. Unta el lomo con la mostaza Dijon por todos lados.
-- 5. En la misma sartén, saltea los champiñones picados hasta que estén dorados y hayan soltado su líquido. Deja enfriar.
-- 6. Extiende la masa de hojaldre sobre una superficie ligeramente enharinada. Coloca el lomo en el centro de la masa y cúbrelo con los champiñones salteados.
-- 7. Envuelve el lomo con la masa, sellando los bordes con el huevo batido.
-- 8. Coloca el lomo envuelto en una bandeja para hornear y barniza la superficie con el huevo batido.
-- 9. Hornea durante aproximadamente 25-30 minutos, o hasta que la masa esté dorada y crujiente.
-- 10. Retira del horno y deja reposar durante unos minutos antes de cortar y servir. ¡Disfruta de tu delicioso lomo al Wellington!