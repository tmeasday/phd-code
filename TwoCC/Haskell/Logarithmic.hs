module Logarithmic
where

import Types
import Random

logProblem :: [Float] -> Integer -> Problem
logProblem list n = (graph, labelling)
  where
    -- take any edge with probability log n / (n-1) [thus log n edges]
    edge_prob = (logBase 2 (fromInteger n)) / (fromInteger (n - 1))
    -- minus edge with probability 1 / log n [thus 1 - edge]
    minus_prob = 1 / logBase 2 (fromInteger n)
    
    graph = map snd $ filter (\(f, _) -> f < edge_prob) (zip list (complete_graph n))
    new_list = drop (fromInteger (n * (n-1) `div` 2)) list
    
    labelling (a,b) = (new_list !! fromInteger (a+b)) > minus_prob