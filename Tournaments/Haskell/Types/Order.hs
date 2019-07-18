module Order
where

import Util

type Order = Int -> Int

-- turn a list of integers into an order
permute :: [Int] -> Order
--permute list n = list !! n
permute list n | n < length list = list !! n
               | otherwise = error $ "Permute gone wrong: " ++ (show n) ++ " " ++ (show $ length list)

allOrders n = map permute (permutations [0..n-1])

showOrd :: Int -> Order -> [Int]
showOrd n o = map o [0..n-1]
