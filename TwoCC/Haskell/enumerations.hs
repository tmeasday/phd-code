module Enumerations where

import Random
import Types

type GraphEnumeration = [Graph]
type RandomGraphGenerator = StdGen -> Graph

randomEnumeration :: RandomGraphGenerator -> Integer -> StdGen -> GraphEnumeration
randomEnumeration _ 0 _ = []
randomEnumeration rgg num gen = (rgg gen1) : randomEnumeration rgg (num - 1) gen2
  where (gen1, gen2) = split gen


thresholdedSelect :: Float -> StdGen -> [a] -> [a]
thresholdedSelect p gen list = map snd $ filter ((<p) . fst) $ zip (randoms gen) list

thresholded :: Float -> Integer -> RandomGraphGenerator
thresholded p n gen = thresholdedSelect p gen (complete_graph n)

