module Main where

import List

-------------------------------------------------------
-- problem type
data Colour = Red | Blue | None
	deriving (Eq, Show)
colours = [Red, Blue, None]
flipColour Red = Blue
flipColour Blue = Red

type Problem = [[Colour]]

size :: Problem -> Int
size g = length g + 1

-- Labellings
data Label = One | Two
	deriving (Show, Eq)
labels = [One, Two]
flipLabel One = Two
flipLabel Two = One
type Labelling = [Label]

generateLabellings :: Int -> [Labelling]
generateLabellings 0 = [[]]
generateLabellings n = [a:l | a <- labels, l <- generateLabellings (n-1)]

generateProbs :: Int -> [Problem]
generateProbs 1 = [[]]
generateProbs n = [r:p | r <- generateRows n, p <- generateProbs (n-1)]
				
generateRows :: Int -> [[Colour]]
generateRows 1 = [[]]
generateRows n = [a:l | a <- colours, l <- generateRows (n-1)]

is_connected p = tsize (bfs_st p 1) == size p

-------------------------------------------------------
-- graph stuff
type Vertex = Int
type Edge = (Vertex, Vertex)

edges :: Problem -> [Edge]
edges p = [(i,j) | i <- [1..size p], j <- [i+1..size p], connected p i j]

to_edge x y = (min x y, max x y)

vertices :: Problem -> [Vertex]
vertices p = [1..size p]

colour :: Problem -> Vertex -> Vertex -> Colour
colour p a b | a < b     = (p !! (a - 1)) !! (b - a - 1)
						 | a == b    = None
             | otherwise = colour p b a

edge_colour :: Problem -> Edge -> Colour
edge_colour p (a,b) = colour p a b

connected :: Problem -> Vertex -> Vertex -> Bool
connected p a b = colour p a b /= None

neighbours :: Problem -> Vertex -> [Vertex]
neighbours p v = filter (connected p v) $ vertices p

-------------------------------------------------------
-- tree stuff

data Tree a = Node a [Tree a]
	deriving (Eq, Show, Ord)

tsize (Node _ ts) = 1 + sum [tsize t | t <- ts]

node_cont :: Tree a -> a
node_cont (Node a _) = a
	
walk_tree :: (Tree a -> b) -> Tree a -> [b]
walk_tree f t@(Node a ts) = f t : concatMap (walk_tree f) ts
	
walk_tree_verts :: (a -> b) -> Tree a -> [b]
walk_tree_verts f = walk_tree (f.node_cont)
	
type LabelledTree a = Tree (a, [Int])
to_lbl_tree :: Tree a -> LabelledTree a
to_lbl_tree t = tlt' t [1]
	where
		tlt' :: Tree a -> [Int] -> LabelledTree a
		tlt' (Node a ts) lbls = Node (a, lbls) ts'
			where ts' = zipWith tlt' ts $ map (\x -> lbls++[x]) [1..]
	
get_lbls :: Tree a -> [[Int]]
get_lbls = walk_tree_verts snd . to_lbl_tree

get_verts = walk_tree_verts id

type SpanningTree = Tree Vertex

bfs_st :: Problem -> Vertex -> SpanningTree
bfs_st p v = fst $ dfs [v] (tail $ bfs_verts_atlvl p v) v
	where
		
		dfs :: [Vertex] -> [[Vertex]] -> Vertex -> (SpanningTree, [Vertex])
		dfs used [] v         = (Node v [], used)
		dfs used (pot:pots) v = (Node v trees, used'')
			where
				kids = filter (connected p v) (pot \\ used)
				used' = used ++ kids
				(trees, used'') = dfs' used' kids
				
				dfs' :: [Vertex] -> [Vertex] -> ([SpanningTree], [Vertex])
				dfs' used []     = ([], used)
				dfs' used (x:xs) = (tree:trees, used'')
					where
						(tree, used')   = dfs used pots x
						(trees, used'') = dfs' used' xs
	
bfs_verts_atlvl :: Problem -> Vertex -> [[Vertex]]
bfs_verts_atlvl p v = [v] : (bva' [] [v])
	where
		bva' p_lvl c_lvl | null kids = []
							       | otherwise = kids : (bva' c_lvl kids)
			where kids = (nub $ concatMap (neighbours p) c_lvl) \\ (c_lvl ++ p_lvl)

get_st_edges :: SpanningTree -> [Edge]
get_st_edges = concat . walk_tree edges_from_node
	where edges_from_node (Node a ts) = [to_edge a (node_cont x) | x <- ts]

tree_lbling :: Problem -> SpanningTree -> Labelling
tree_lbling p = tl' One
	where
		tl' l (Node v ts) = l : (concat $ zipWith tl' newLs ts)
			where 
				newLs = [if colour p v (node_cont t) == Blue then l else flipLabel l | t <- ts]

-------------------------------------------------------
-- PAST stuff
past_single :: Problem -> SpanningTree -> Labelling
past_single p t = reorder_by (get_verts t) $ tree_lbling p t

reorder_by :: [Vertex] -> [a] -> [a]
reorder_by pos xs = map rb' [1..length pos]
	where rb' i = xs !! head (elemIndices i pos)

-------------------------------------------------------
-- PASTA_flip stuff

pasta_single :: Problem -> SpanningTree -> Labelling
pasta_single p t = search p
	where 
		all_edges   = edges p
		st_edges    = get_st_edges t
		cycle_edges = all_edges \\ st_edges
		node_lbls   = reorder_by (get_verts t) (get_lbls t)
		min_bad     = (size p - 2) `div` 2
		zeros       = replicate (length (st_edges)) 0

		search :: Problem -> Labelling
		search p | worst <= 0 = curr_lbls
		         | otherwise  = search (flipColours e p)
		  where
				curr_lbls = past_single p t
				
				errs = foldr (zipWith (+)) zeros $ map cyc_errs cycle_edges
				worst = maximum errs
				e = st_edges !! head (elemIndices worst errs)
	
				cyc_errs :: Edge -> [Int]
				cyc_errs (a,b) | corr      = map ((-1)*) edges_ic
											 | otherwise = edges_ic
				  where
						corr | colour p a b == Blue = curr_lbls !! (a-1) == curr_lbls !! (b-1)
								 | otherwise            = curr_lbls !! (a-1) /= curr_lbls !! (b-1)
						nla = node_lbls !! (a-1)
						nlb = node_lbls !! (b-1)
					 	short_p = common_pref nla nlb
				
						lbl_nos = map lbl_no node_lbls
						lbl_no x | length x < short_p = -2
										 | x == take (length x) nla = length nla - length x
						         | x == take (length x) nlb = length nla + length x - 2 * short_p
						         | otherwise = -2
				
						edges_ic = map edge_in_c st_edges
						edge_in_c (a,b) | abs (lbl_nos !! (a-1) - lbl_nos !! (b-1)) == 1 = 1
														| otherwise                                  = 0
					
		common_pref xs ys = length $ takeWhile id $ zipWith (==) xs ys
			
--			e = maxBy (nBadEdge g') edges
			

		flipColours :: Edge -> Problem -> Problem
		flipColours (i, j) p | i > j = flipColours (j, i) p
		flipColours (i, j) p = prefix ++ [newRow] ++ suffix
			where
				(prefix, (oldRow:suffix)) = splitAt (i-1) p
		
				newRow = rowPre ++ [flipColour c] ++ rowSuff
				(rowPre, (c:rowSuff)) = splitAt (j-i-1) oldRow

cost :: Problem -> Labelling -> Int
cost p lbls = length [e | e <- edges p, not $ isCorrect e]
	where 
		isCorrect (i,j) | colour p i j == Blue = lbli == lblj
		 								| otherwise = lbli /= lblj
			where 
				lbli = lbls !! (i-1)
				lblj = lbls !! (j-1)

optCost :: Problem -> Int
optCost p = minimum costs
	where
		lblings = generateLabellings (size p)
		costs = map (cost p) lblings
	
pasta_cost p = minimum costs
			where 
				costs = [cost p $ pasta_single p $ bfs_st p v | v <- vertices p]
	
pasta_ratio p = (pasta_cost p, optCost p)
			
pasta_check p = x /= y
	where (x,y) = pasta_ratio p

test = [[Red, Red, None, None, None, None], [None, Red, Red, Red, None], [None, None, Red, Red], [None, None, None], [None, Red], [None]]
easy = [[Red,None], [Red]]


