import System
import Text.Printf
import RandomList
import List

main = do
  args <- getArgs
  let (n:m:_) = map read args
  rands <- randomList 
  let res   = mapR (run) (replicate m n) rands
  let res'  = foldr (zipWith (+)) (replicate n 0) res
  let res'' = map ((/(fromIntegral m)).fromIntegral) res'
  print res''


run :: Int -> [Float] -> [Int]
run n rands = map (getCliques graph) [1..n]
  where graph = random_graph n rands

random_graph :: Int -> [Float] -> [[Bool]]
random_graph n rands = mapR random_line (reverse [1..n-1]) rands

random_line :: Int -> [Float] -> [Bool]
random_line n rands  = mapR (\_ (x:_) -> x > 0.5) [1..n] rands

printG :: [[Bool]] -> IO [()]
printG list = mapM (print) list

getCliques :: [[Bool]] -> Int -> Int
getCliques graph k = getCliques' graph k [] 0
  where getCliques' _ 0 _ _ = 1
        getCliques' graph _ _ n | length graph < n = 0
        getCliques' graph k members next = n_with + n_without
          where n_with | connected graph next members 
                    = getCliques' graph (k-1) (next:members) (next+1)
                       | otherwise = 0
                n_without = getCliques' graph k members (next+1)

connected :: [[Bool]] -> Int -> [Int] -> Bool
connected graph x ys = and $ map (edge graph x) ys

edge graph x y = graph !! y !! (x-y-1)
