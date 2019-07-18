module LocalTourn where
import Tournaments
import LocalSearch
import Util
import Upsets
import Order

import List

n = 5 :: Int
p = 0.7 :: Float

--import RandomList
--run = do rs <- randomList 
	 --let (t, rs') = generateTourn n p rs
	 --let o = permute [0..n-1]
	 --let o' = hillClimb (\x -> map (x.) (singleMoves n)) (upsets t) o
	 --print t
	 --print $ showOrd n o'
	 --print $ upsets t o'
	 --print $ optimalCost t



-- LOCAL SEARCH STUFF

-- Like a generator for local moves
type LocalMove   = Order 
type LocalScheme = Int -> [LocalMove]

singleMoves :: LocalScheme
singleMoves n = [singleMove a b | a <- [0..n-1], b <- [0..n-1], a /= b]

-- move n to position m (or is it the other way?)
singleMove :: Int -> Int -> LocalMove
singleMove n m i | i == n          = m
                 | i > n && i <= m = i-1 -- (n < m) 
                 | i < n && i >= m = i+1 -- (n > m)
                 | otherwise       = i

-- swap two guys
swaps :: LocalScheme
swaps n = [swap a b | a <- [0..n-1], b <- [a+1..n-1]]

swap :: Int -> Int -> LocalMove
swap n m i | i == n    = m
           | i == m    = n
           | otherwise = i

-- 'multi-dimensional' swaps (rearrangements 2 = swaps)
-- fixme there is a fair bit of redundancy here
rearrangements :: Int -> LocalScheme
rearrangements k n = [move p s | s <- subsets k [0..n-1], p <- permutations s]
  where move p s i = case elemIndex i s of 
             Just j -> p !! j
             Nothing -> i
        
-- ok we aren't going to be too smart here
localOpt :: PairwiseData a => LocalScheme -> a -> Order -> Bool
localOpt moves tourn order = and $ map ((cost <=).(upsets tourn)) orders
  where cost = upsets tourn order 
        orders = map (order.) (moves $ size tourn)

localOrders :: PairwiseData a => LocalScheme -> a -> [Order]
localOrders moves t = [o | o <- allOrders (size t), localOpt moves t o]
