module Algorithms where
import Util
import Types
import Data.Function
import Data.List

type ClusteringAlgorithm = Problem -> Clustering
type BoundedClusteringAlgorithm = Int -> ClusteringAlgorithm

-- find the best clustering by brute force
optimum :: ClusteringAlgorithm
optimum p = best_clustering p $ all_clusterings (fst p)

-- find all the labellings induced by all the dfs trees from each vertex
past :: ClusteringAlgorithm
past p = best_clustering p $ past_clusterings p

past_clusterings p = concatMap (dfs_clusterings p) $ vertices (fst p)

-- a bounded version of past
pastBound :: BoundedClusteringAlgorithm
pastBound bound p = best_bounded_clustering bound p $ past_clusterings p

-- find the best of a list of clusterings
best_clustering :: Problem -> [Clustering] -> Clustering
best_clustering p cs = snd $ minimumBy (compare `on` fst) $ [(cost p c, c) | c <- cs]

-- as above, but with a bound
best_bounded_clustering :: Int -> Problem -> [Clustering] -> Clustering
best_bounded_clustering bound p cs = snd $ minimumByBound fst bound $ [(cost p c, c) | c <- cs]

dfs_clusterings :: Problem -> Vertex -> [Clustering]
dfs_clusterings p v = build_clusterings [[(v, True)]] [v] [base_c]
  where
    -- third argument is the levels of the tree (as constructed so far)
    --   with the clusters (t/f) attached (memberships)
    -- fourth argument is the vertices we've seen
    -- last argument is clustering accumulators
    build_clusterings :: [[(Vertex, Bool)]] -> [Vertex] -> [Clustering] -> [Clustering]
    build_clusterings [] _ cs = cs
    -- finish a level of the tree
    build_clusterings (ms:mms) used cs 
                      | ms == []  = build_clusterings mms used cs
                      | otherwise = concat [ choose sel | sel <- selections ms]
      where 
        choose (m,ms_rem) = build_clusterings mms' used' cs'
          where
            -- the new cluster memberships
            ms'   = look_from p used m
            mms'  = (ms_rem:mms) ++ [ms'] -- m has been removed, ms' added
            used' = (map fst ms') ++ used -- we've now seen vs'
            -- update each clustering with the cluster given by ms'
            cs'   = [foldr (uncurry update_c) c ms' | c <- cs]
    
    look_from :: Problem -> [Vertex] -> (Vertex, Bool) -> [(Vertex, Bool)]
    look_from (g, l) used (u, b) = [ (v, b == (l $ edge u v))
                                | v <- neighbours g u,
                                  v `notElem` used]


dfs_trees :: Graph -> Vertex -> [Tree]
dfs_trees g v = remove_duplicates $ adt' g [[v]] [v] [[]]
  where
    -- adt' graph frontier used so_far
    adt' :: Graph -> [[Vertex]] -> [Vertex] -> [Graph] -> [Graph]
    adt' _ [] _ ts = ts
    adt' g ([]:vvs) used ts = adt' g vvs used ts -- finish a level of the tree
    adt' g (vs:vvs) used ts = concat [ choose sel | sel <- selections vs]
      where 
        choose (v,vs_rem) = adt' g vvs' used' ts'
          where (es, vs') = look_from g used v   -- all the edges and vs that we found
                vvs'      = (vs_rem:vvs) ++ [vs'] -- v has been removed, vs' added
                used'     = vs' ++ used          -- we've now seen vs'
                ts'       = map (++es) ts      -- new trees
            
    look_from :: Graph -> [Vertex] -> Vertex -> ([Edge], [Vertex])
    look_from g used u = unzip [((u, v), v) | v <- neighbours g u,
                                              v `notElem` used]

-- get the clustering induced by t on g
--   assumption node 1 in in cluster 1 (true)
t2c :: Tree -> Labelling -> Clustering
  -- are an even number of edges labelled true?
t2c t l v = even $ length $ filter l $ path_to_1 t v 

-- find the path from a to b in the tree
-- path :: Tree -> Vertex -> Vertex -> Path
-- path t v1 v2

path_to_1 :: Tree -> Vertex -> Path
path_to_1 _ 1 = []
path_to_1 t v = (edge v v') : path_to_1 t v'
  where v' = tree2adj t v -- ancestor of v 

-- we are rooting the tree at 1
-- and this outputs a function tells us the ancestor of v
tree2adj :: Tree -> (Vertex -> Vertex)
tree2adj tree = adj_builder tree id
  where
    adj_builder :: Tree -> (Vertex -> Vertex) -> (Vertex -> Vertex)
    adj_builder [] adj = adj
    adj_builder ((v1, v2):es) adj = adj_builder es (\x -> if x == v2 then (adj v1) else (adj x))
