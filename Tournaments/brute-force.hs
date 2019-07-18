import List
import Ratio
import System

main = do args <- getArgs
          print (head args)

run moves n m = argmax (\(a,_,_) -> a) $ showWorst moves n m

showWorst moves n m = [showWorst' t | t <- allBipartite n m]
  where showWorst' t = (r, t, showOrd (size t) o) 
          where (r, o) = worstLocalOpt moves t

-- Do this by tournament
worstLocalOpt :: PairwiseData a => LocalScheme -> a -> (Int, Order)
worstLocalOpt moves t = (upsets t o - best, o)
  where o    = argmax (upsets t) (localOrders moves t)
        best = optimalCost t




{-
worstTourn n = maxAndArg (localTourn n) ratio

minPerm tourn = minAndArg (permutations [0..length tourn]) (upsets tourn)

minCost tourn = minimum $ map (upsets tourn) (permutations [0..length tourn])

ratio tourn | cost /= 0  = cost % minCost tourn
  where cost = (upsets tourn [0..(length tourn)])
ratio tourn | otherwise  = 1 % 1

-}
{-

badSoln n = [2*i + 1 | i <- [0..half]] ++ [2*i | i <- [0..half]]
  where half = n `div` 2

localOpt tourn = checkRows tourn && checkCols tourn
  where
    -- the rows tell us what is going into a node
    checkRows tourn = and $ map checkRow tourn

    -- the cols tell us what is going out of a node
    checkCols tourn = and $ map checkRow (reverseTourn tourn)

    checkRow  row   = checkRow' row 0
    checkRow' _ n | n < 0 = False
    checkRow' [] _  = True
    checkRow' (True:bs) n = checkRow' bs (n+1)
    checkRow' (False:bs) n = checkRow' bs (n-1) 


-- make the 'opposite tournament' (reverse the order, reverse all edges)
reverseTourn tourn = reverse $ reverseTourn' tourn 1
  where
    reverseTourn' ([]:_) _ = [] 
    reverseTourn' tourn n = reverse front : reverseTourn' end (n+1)
      where
        front = concat $ map (take 1) (take n tourn)
        end   = map (drop 1) (take n tourn) ++ drop n tourn

-}
