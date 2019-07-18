module Types where
import List
import Maybe
import Util

type Vertex = Integer
type Edge = (Vertex, Vertex)

type Graph = [Edge]
-- specialisations
type Tree = Graph
type Path = Graph

vertices g = let (a,b) = unzip g
  in sort $ nub $ a ++ b

edge v v' = (if v < v' then (v, v') else (v', v))


neighbours :: Graph -> Vertex -> [Vertex]
neighbours g v = [u | (u, x) <- g, x == v] ++ [u | (x, u) <- g, x == v]

-- normalize a graph-- get edges in the right order and the right way round
normalize :: Graph -> Graph
normalize g = sort [if u <= v then (u,v) else (v,u) | (u,v) <- g]

remove_duplicates :: [Graph] -> [Graph]
remove_duplicates gs = nub $ map normalize gs

-- we only deal with 2 clusters, so true/false indicates different clusters
type Labelling = Edge -> Bool

type Clustering = Vertex -> Bool

type Problem = (Graph, Labelling)

base_c = (\_ -> True)
update_c :: Vertex -> Bool -> Clustering -> Clustering
update_c v cluster c v' | v == v'   = cluster
                        | otherwise = c v'

-- the TwoCC cost function
cost :: Problem -> Clustering -> Int
cost (es, l) c = count sat es
  where sat e@(v1, v2) = l e == ((c v1) /= (c v2))
        
count :: (a -> Bool) -> [a] -> Int
count f = foldr (\x -> if f x then (1 +) else id) 0


-- get an array version of a labelling/clustering based on a graph
l2a g l = map l g
a2l g a e = fromJust $ lookup e $ zip g a

c2a g c = map c $ vertices g
-- do we need to pass in g?
a2c g a v = fromJust $ lookup v $ zip (vertices g) a

-- a_graph = [(1,2), (1,3), (1,4), (2,3), (3,4)] 
a_graph = [(1,2), (1,3), (2,3)]
-- a_labelling_a = [True, False, True, False, True]
a_labelling_a = [True, True, False]
a_labelling = a2l a_graph a_labelling_a
a_clustering_a = [True, False, True, True]
a_clustering = a2c a_graph a_clustering_a

a_problem = (a_graph, a_labelling)
a_tree = [(1,3),(2,3)]

-- generate all graphs over 1..n
complete_graph n = [(u, v) | u <- [1..n], v <- [(u+1)..n]]
base_clustering n v = True
all_graphs n = subsets (complete_graph n)

all_labellings g = map (a2l g) $ all_bool_lists g
all_clusterings g = map (a2c g) $ all_bool_lists $ vertices g

all_problems n = [(g,l) | g <- all_graphs n, l <- all_labellings g]

-- easy test of whether a problem is in the interesting switching class
is_candidate :: Problem -> Bool
is_candidate (g, l) = all is_plus_vertex (vertices g)
  where
    is_plus_vertex v = (foldr ((+).b2i.l.(edge v)) 0 (neighbours g v)) >= 0 
    b2i True = 1
    b2i False = -1