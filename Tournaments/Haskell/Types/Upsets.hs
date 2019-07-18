module Upsets 
where

import Tournaments
import List
import Order

-- dammit have to write a new upsets for weighted
-- doesn't seem to work...

--upsets :: PairwiseData a => a -> Order -> Float
--upsets t o = (sum $ map upsets' $ nodes t)
--  where 
--    -- cant be bothered thinking here
--    ps = showOrd (size t) o
--    revps = revPerm ps
--
--    upsets' pos = sum $ filter (>0) $ zipWith (*) above edgeWeights
--      where p = ps !! pos
--            laterps = (drop (p+1) $ nodes t)
--
--            edgeWeights = map (beats t p) laterps
--            above = map (\x -> if revps!!x < pos then 1 else -1) laterps

revPerm :: [Int] -> [Int]
revPerm ps = map (unMaybe . (\p -> elemIndex p ps)) [0..length ps-1]

unMaybe :: Maybe a -> a
unMaybe (Just x) = x

 --old, boolean version
upsets :: PairwiseData a => a -> Order -> Int
upsets tourn ord = sum $ map upsets' players
 where
    players = map ord [0..size tourn-1]
    upsets' p = length $ filter better ps 
      where ps = takeWhile (/= p) players
            better p' = beats tourn p p' == 1
            
--optimalCost tourn = upsets tourn (optimalOrder tourn)
optimalOrder tourn = argmin (upsets tourn) (allOrders (size tourn))

argmin :: Ord b => (a -> b) -> [a] -> a
argmin f (i:inputs) = argmin' f inputs i (f i)
	where 
		argmin' _ [] i _ = i
		argmin' f (i:inputs) best c | f i < c = argmin' f inputs i (f i)
		                            | otherwise = argmin' f inputs best c