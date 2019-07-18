module Test where

import Types
import Algorithms
import Util
import Data.Function
import Data.List

-- note: assumes the optimum clustering places all nodes in a single cluster
upperBound :: BoundedClusteringAlgorithm -> [Problem] -> (Float, (Problem, Clustering))
upperBound alg ps = maximumBy (compare `on` fst) $ map test ps
  where
    test p = (score, (p, c))
      where
        c        = alg 1 p
        opt_star = cost p base_c
        c_cost   = cost p c
        score    | opt_star == 0 && c_cost == 0 = 1
                 | otherwise = (fromIntegral $ c_cost) / (fromIntegral $ opt_star)
    
-- to display a bound
showBound :: (Float, (Problem, Clustering)) -> String
showBound (s, ((g, l), c)) = show s ++ ": " ++ show g ++ show (l2a g l) ++ 
                                 show (c2a g c)