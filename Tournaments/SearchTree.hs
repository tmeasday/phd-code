module SearchTree where
import Tournaments
import List


-- these are directed edges
type Edges = [(Int, Int)]

-- I'm not trying to be smart here (i.e. by saving the connection data)
-- but does haskell do it for me? <- check this

pathExists :: Edges -> Int -> Int -> Bool
pathExists es a b = elem b (pathDescendants es a) 

pathDescendants :: Edges -> Int -> [Int]
pathDescendants es a = desc' es [a] []

  -- TODO -- this is pretty ugly
  -- if we get to the point where things aren't changing anymore
  where desc' _ list list' | list==list' = list
        desc' es list _ = desc' es list'' list

          where list' = foldr union [] $ map (pathChildren es) list
                list'' = union list list'


pathChildren :: Edges -> Int -> [Int]
pathChildren es a = [y | (x,y) <- es, x==a ]


data Multitree a = Node a [Multitree a]
  deriving Eq

instance (Show a) => Show (Multitree a) where
  show (Node info nodes) = foldr ((++).('\n':)) "" lines 
  
    where lines = show info : concatMap (show' 1) nodes
          
          show' n (Node info nodes) = (prefix ++ show info) : lines
            where prefix = replicate (n-1) ' ' ++ "|-"
                  lines = concatMap (show' (n+1)) nodes


level :: Int -> Multitree a -> [a]
level n tree = [a | Node a _ <- levelTrees n tree]

levelTrees :: Int -> Multitree a -> [Multitree a]
levelTrees 0 node           = [node]
levelTrees _ (Node _ [])    = []
levelTrees n (Node _ nodes) = concatMap (levelTrees (n-1)) nodes   

traverse :: (a -> [b] -> b) -> b -> Multitree a -> b
traverse f c (Node a [])    = c
traverse f c (Node a nodes) = f a (map (traverse f c) nodes)

-- useful for traverse
second = flip const

depth :: Multitree a -> Int
depth = traverse (\_ bs -> maximum bs + 1) 0

minDepth :: Multitree a -> Int
minDepth = traverse (\_ bs -> minimum bs + 1) 0
     
-- for interest mainly
size :: Multitree a -> Int
size = traverse (\_ bs -> sum bs + 1) 1


type SearchTree = Multitree Edges


-- What are the possible children of this set of edges?
children :: PairwiseData a => a -> Edges -> [Edges]
children t es = [((a,b):es) | a <- nodes t, b <- losers t a, 
                 not $ elem (a,b) es, not $ pathExists es b a]

buildSearchTree :: PairwiseData a => a -> SearchTree
buildSearchTree t = buildST' t []
  where buildST' t soln = Node soln [buildST' t s | s <- children t soln]


type ProbTree = Multitree Float
probTree :: Multitree a -> ProbTree
probTree t = buildPT' t 1.0
  where buildPT' (Node _ nodes) p = Node p [buildPT' n p' | n <- nodes]
          where p' = p / (fromIntegral $ length nodes)

buildProbTree :: PairwiseData a => a -> ProbTree
buildProbTree = probTree . buildSearchTree
        
a :: Int -> ProbTree -> Float
a i tree = sum $ level i tree

as :: ProbTree -> [Float]
as tree = takeWhile (/= 0) $ [a i tree | i <- [0..]]

kij :: ProbTree -> Int -> Int -> Float
kij tree i j = ((a i tree)/(a j tree)^2) * kij'
  where kij' = sum [ (1/p) * (a (j-i) t)^2 | t@(Node p _) <- levelTrees i tree ]
-- is 1/p right here? as a of a subtree is p * a
     

ks tree = [kij tree i j | i <- [1..d], j <- [i..d]]
  where d = depth tree

kappa :: ProbTree -> Float
kappa tree = maximum $ ks tree

bad4PT = buildProbTree bad4Tourn
triPT  = buildProbTree triangleTourn

myST = Node 1 [(Node 1 [(Node 1 []), (Node 1 [(Node 1 [(Node 1 [])])])]), (Node 1 [(Node 1 [(Node 1 [(Node 1 [])]), Node 1 []]), (Node 1 []), (Node 1 [])])]

myPT = probTree myST

maxArat t = maximum $ [aj / aj1 | aj <- list | aj1 <- tail list]
  where list = as t

getDataFromTourn :: PairwiseData a => a -> String
getDataFromTourn d = "size: " ++ (show $ SearchTree.size t) ++
                     ", depth: " ++ (show $ depth t) ++
                     ", minDepth: " ++ (show $ minDepth t) ++
                     ", kappa: " ++ (show $ kappa t) ++ 
                     ", maxArat: " ++ (show $ maxArat t)
  where t = buildProbTree d
