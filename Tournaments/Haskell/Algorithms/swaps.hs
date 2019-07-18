import Process
import LocalSearch
import LocalTourn
import Upsets
import Tournaments
import Order

main = process order

order :: [[Float]] -> [Int]
order array = showOrd n $ hillClimb neigh cost state where
  n = length array
  neigh = (\x -> map (x.) (swaps n))
  cost  = (upsets (WT array))
  state = permute [0..n-1]
