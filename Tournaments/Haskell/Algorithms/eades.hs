import Process

main = process order

order :: [[Float]] -> [Int]
order tourn = qsort [0..size-1] where
  size = length tourn

  qsort [] = []
  qsort (x:xs) = qsort before ++ [x] ++ qsort after where
    before = [y | y <- xs, tourn!!x!!(y-x-1) <= 0] 
    after  = [y | y <- xs, tourn!!x!!(y-x-1) >  0]
