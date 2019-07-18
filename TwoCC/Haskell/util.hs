module Util where


all_bool_lists :: [x] -> [[Bool]]
all_bool_lists l = cartesian $ map (\_ -> [True, False]) l

cartesian :: [[a]] -> [[a]]
cartesian [] = [[]]
cartesian (list:lists) = [(x:xs) | x <- list, xs <- cartesian lists]

subsets :: [a] -> [[a]]
subsets [] = [[]]
subsets (x:xs) = (subsets xs) ++ (map (x:) (subsets xs))

selections                    :: [a] -> [(a, [a])]
selections []                 =  []
selections (x:xs)             =  (x,xs) : 
                                 [(y,x:ys) | (y,ys) <- selections xs]

minimumByBound :: Ord b => (a -> b) -> b -> [a] -> a
minimumByBound f bound (x:xs) = mBB xs (x, f x)
  where
    -- stop if we hit the bound
    mBB _ (min, score) | score <= bound = min
    mBB [] (min, _) = min
    mBB (x:xs) (min, min_s) | f x < min_s = mBB xs (x, f x)
                            | otherwise   = mBB xs (min, min_s)