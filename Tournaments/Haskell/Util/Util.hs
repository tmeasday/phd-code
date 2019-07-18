module Util
where

import List

-- UTILITIES
argcmp :: (a -> a -> Bool) -> [a] -> a
argcmp cmp = foldr1 cmp'
  where cmp' x y | cmp x y   = x
                 | otherwise = y

argmax :: Ord b => (a -> b) -> [a] -> a
argmax f = argcmp (\a b -> f a > f b)

argmin :: Ord b => (a -> b) -> [a] -> a
argmin f = argcmp (\a b -> f a < f b)

-- all subsets of size k
subsets :: Int -> [a] -> [[a]]
subsets 0 _      = [[]]
subsets _ []     = []
subsets k (x:xs) = map (x:) (subsets (k-1) xs) ++ subsets k xs 

perms [] = [[]]
perms xs = [ x : ps | x <- xs , ps <- perms ( xs \\ [x]) ]

selections                    :: [a] -> [(a, [a])]
selections []                 =  []
selections (x:xs)             =  (x,xs) : 
                                 [(y,x:ys) | (y,ys) <- selections xs]

permutations                  :: [a] -> [[a]]
permutations []               =  [[]]
permutations xs               =  [y:zs | (y,ys) <- selections xs
                                       , zs     <- permutations ys] 
                                      
dropIndex :: Int -> [a] -> [a]
dropIndex i list = (take i list) ++ (drop (i+1) list)

--clearly not my code
unique xs =                     -- remove duplicates
 let
   r = u xs 0                         -- result list
   u []     _ = []                    -- build result
   u (x:xs) n = if member x r n then u xs n
                      else x:(u xs (n+1))
   member e xs     0 = False
   member y (x:xs) n = x==y || member y xs (n-1)
 in r

