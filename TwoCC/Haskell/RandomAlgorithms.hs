module RandomAlgorithms where
import Types
import List
import Maybe
import Debug.Trace

-- todo -- we could work out the cost as we go, and not have to keep anything in memory
-- return a clustering if the problem is connected
random_dfs_clustering :: [Float] -> Problem -> Vertex -> Maybe Clustering
random_dfs_clustering rands (g,l) v = rdc rands [v] [v] [] base_c
  where
    -- generate a random dfs clustering of p, with arguments
    -- 1: list of rands
    -- 2: who have we seen?
    -- 3: current level of the tree we are investigating
    -- 4: next level to investigate
    -- 5: clustering so far
    rdc :: [Float] -> [Vertex] -> [Vertex] -> [Vertex] -> Clustering -> Maybe Clustering
    -- rdc _ vs level next_level _ | trace ("rdc: " ++ show vs ++ " - " ++ show level ++ " - " ++ show next_level) False = undefined
    -- the clustering is finished-- we are out of levels
    rdc _ vs  [] [] c = if sort vs == vertices g then Just c else Nothing
    -- this level is finished
    rdc rs vs [] next_level c = rdc rs vs next_level [] c
    -- choose a root from this level
    rdc (r:rs) vs level next_level old_c = rdc rs (vs++found) level' (next_level++found) new_c
      where
        -- pick a random guy from this level
        root = level !! (floor r*(fromIntegral $ length level))
        level' = delete root level
        
        -- who are we adding to the tree ?
        found = neighbours g root \\ vs
                
        -- the clustering you get from combining c + c' with what we've just found
        --   we know the root has been clustered by old_c, we can use edge to cluster v
        new_c v | elem v found = old_c root == l (edge root v) -- xnor
                | otherwise    = old_c v