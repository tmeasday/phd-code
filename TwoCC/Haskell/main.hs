module Main where

import System
import Types
import Algorithms
import Test
import Enumerations
import Random
import Logarithmic
import RandomAlgorithms
import List
import Maybe

-- import RandomT

main = do
  args <- getArgs
  let n = read $ head args
  let reps = read $ args !! 1
  gen <- getStdGen
  let (gen', gen'') = split gen
  let problems = generate_problems gen' reps (\g -> logProblem (randoms g) n)

  let info = map_random gen'' solve problems
  let worst = maximumUsing (\(x,_,_,_,_) -> x) $ catMaybes info

  print $ (\(r, c1, c2, c, p@(g,l)) -> (r, c1, c2, c2a g c, g, l2a g l)) worst

-- solve :: (Floating f, Integral i) => StdGen -> Problem -> Maybe (f, i, i, Clustering, Problem)
solve gen p@(g,l) = case head solns of 
                     Nothing -> Nothing
                     _ -> Just output
  where
    solns = map_random gen (\gen' v -> random_dfs_clustering (randoms gen') p v) (vertices g)
    (cst, soln) = minimumUsing fst $ map (\(Just s) -> (cost p s, s)) solns
    opt_cost = cost p base_c
    output = ((fromIntegral cst) / (fromIntegral opt_cost), cst, opt_cost, soln, p)

show_soln _ Nothing = "Graph not connected"
show_soln g (Just x) = show $ c2a g x
show_cost _ Nothing = "Graph not connected"
show_cost p (Just x) = show $ cost p x
  
map_random _ _ [] = []
map_random g f (x:xs) = (f g' x): map_random g' f xs
  where (g', g'') = split g

generate_problems g 0 pgen = []
generate_problems g c pgen = (pgen g'): generate_problems g'' (c-1) pgen
  where (g', g'') = split g
  
maximumUsing f = maximumBy (\a b -> compare (f a) (f b))
minimumUsing f = minimumBy (\a b -> compare (f a) (f b))