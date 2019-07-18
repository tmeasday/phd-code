import Process
import List

main = process score

--score :: [[Float]] -> Float
score tourn = order
  where size  = length tourn
        indeg = indegrees tourn
        order = [e | (_, e) <- reverse $ sort $ zip indeg [0..size-1]]


indegrees :: [[Float]] -> [Float]
indegrees t = ind'  t (replicate (length t) 0)
  where ind' [] _ = []
        ind' (es:t) (d:ds) = (sum es + d): ind' t ds'
          where ds' = zipWith (-) ds es
              

