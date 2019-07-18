module Tournaments where
import Util

-- Pairwise data is indexed by 0-n where n is the size of the tournament
-- (number of nodes). w = beats x y is > 0 if x beats y with weight w.
-- otherwise it is 0.

class PairwiseData a  where
  beats        :: a -> Int -> Int -> Float
  size         :: a -> Int
  remove       :: a -> Int -> a

nodes :: PairwiseData a => a -> [Int]
nodes t = [0..size t-1]

-- beats1 means really did beat
beats1 :: PairwiseData a => a -> Int -> Int -> Bool
beats1 t a b | beats t a b > 0 = True
             | otherwise       = False
  
-- who beat x?
winners :: PairwiseData a => a -> Int -> [Int]
winners t x = [y | y <- nodes t, beats1 t y x]

-- who lost to x?
losers :: PairwiseData a => a -> Int -> [Int]
losers t x = [y | y <- nodes t, beats1 t x y]

-- degrees of tournaments
indegree :: PairwiseData a => a -> Int -> Float
indegree tourn p = sum $ map (beats tourn p) (nodes tourn)
  
outdegree :: PairwiseData a => a -> Int -> Float
outdegree tourn p = sum $ map (\x -> beats tourn x p) (nodes tourn)


-- triangular list of lists
data Tournament = T [[Bool]]
  deriving (Eq, Show)

-- will be useful twice
b2i True  = 1
b2i False = 0 


removeTri :: [[a]] -> Int -> [[a]]
removeTri list index = front ++ back where
  front = strip index (take index list) 
  back  = drop (index+1) list
  strip _ [] = []
  strip i (l:ls) = dropIndex i l : strip (i-1) ls

instance PairwiseData Tournament where
  beats (T array) p1 p2 | p1 < p2  = b2i $ array !! p1 !! (p2 - p1 - 1)
                        | p1 > p2  = b2i $ not $ array !! p2 !! (p1 - p2 - 1)
                        | p1 == p2 = 0

  remove (T array) p = T $ removeTri array p
                        
  size (T array) = length array + 1

-- examples
allRows :: Int -> [[Bool]]
allRows 0 = [[]]
allRows n = map (True:) rows ++ (map (False:) rows)
  where rows = allRows (n-1)

-- all instances here is waay to general
allTourns 0 = [T []]
allTourns n = [T (r:t) | r <- (allRows n), T t <- (allTourns (n-1))]

-- Some examples

simplestTourn = T [[True]]
triangleTourn = T [[True, False], [True]]
bad4Tourn = T [[True, False, True], [True, False], [True]]
perfectTourn n = T [replicate i True | i <- reverse [1..n]]

evenTourn n = T $ map evenRow (reverse [1..n])

evenRow 0 = []
evenRow 1 = [True]
evenRow n = [True, False] ++ evenRow (n-2)

oddTourn n = T $ map oddRow (reverse [1..n])

oddRow n = True : evenRow (n-1)

badTourn n = T [badRow i | i <- reverse [1..n]]

badRow n | n `mod` 2 == 0 = trueRow n
badRow n                  = oddRow n

trueRow n = replicate n True


-- square list of lists
data BipartiteTourn = BT [[Bool]]
  deriving (Eq, Show)

dim (BT array) = (length array, length $ head array)

-- not the most efficient
allBipartite :: Int -> Int -> [BipartiteTourn]
allBipartite 0 _   = [BT []]
allBipartite n1 n2 = [BT (r:t) | r <- (allRows n2), BT t <- (allBipartite (n1 - 1) n2)]

allBP n = concat [allBipartite i (n-i) | i <- [1 .. n `div` 2]]

instance PairwiseData BipartiteTourn where
  beats t@(BT a) p1 p2 | p1 < n1 && p2 >= n1 = b2i $ a !! p1 !! (p2 - n1)
                       | p2 < n1 && p1 >= n1 = b2i $ not $ a !! p2 !! (p1 - n1)
    where (n1, n2) = dim t
  beats t _ _ = 0

  size bptourn = n1 + n2
    where (n1, n2) = dim bptourn

  remove t@(BT a) p | p < n1    = BT $ dropIndex p a
                    | otherwise = BT $ map (dropIndex (p-n1)) a
    where (n1, n2) = dim t

-- triangular list of lists
data WeightedTournament = WT [[Float]]
  deriving Eq

instance PairwiseData WeightedTournament where
  beats (WT array) p1 p2 | p1 < p2  = array !! p1 !! (p2 - p1 - 1)
                         | p1 > p2  = 1 - array !! p2 !! (p1 - p2 - 1)
                         | p1 == p2 = 0

  remove (WT array) p = WT $ removeTri array p

  size (WT array) = length array

instance Show WeightedTournament where
  showsPrec x (WT array) = showsPrec x array

instance Read WeightedTournament where
  readsPrec x str = [(WT array, str') | (array, str') <- (readsPrec x str)]

