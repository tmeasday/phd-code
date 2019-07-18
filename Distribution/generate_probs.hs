import System
import Text.Printf

fact :: Int -> Int
fact n = product [1..n]

perm :: Int -> Int -> Int
perm n r = product [(n-r)+1 .. n]

binoms = iterate binoms' [1]
  where binoms' list = 1:(zipWith (+) list (tail list)) ++ [1]

binom :: Int -> Int -> Integer
binom n r = binoms !! n !! r

bernoulli :: Int -> Int -> Float
bernoulli n k = fromIntegral (binom n k) * (2 ** (fromIntegral $ -1*n))

main = do
  (n':m':_) <- getArgs
  let n = read n'
  let m = read m'
  mapM (printLine n m) [0..n]

printLine n m k = do
  printf "%2d: %f\n" k $ m * bernoulli n k
 

