module Main where

import List
import Ratio

data Colour = Red | Blue
	deriving (Eq, Show)
	
colours = [Red, Blue]


-- triangular
type RBGraph = [[Colour]]

bad_example n = flipColours (1, n) $ map bad_row $ reverse [1..n-1]
	where bad_row i = Red : replicate (i-1) Blue

save = [[Red,Red,Red,Blue,Blue,Blue],[Blue,Blue,Red,Red,Blue],[Blue,Red,Red,Blue],[Red,Red,Blue],[Blue,Blue],[Blue]]

size :: RBGraph -> Int
size g = length g + 1


-- indexed from 1
type Edge = (Int, Int)
type Vertex = Int

edges :: RBGraph -> [Edge]
edges g = [(i,j) | i <- [1..size g], j <- [i+1..size g]]

vertices :: RBGraph -> [Vertex]
vertices g = [1..size g]

generateRBs :: Int -> [RBGraph]
generateRBs 1 = [[]]
generateRBs n = [r:g | r <- generateRows n, g <- generateRBs (n-1)]
				
generateRows :: Int -> [[Colour]]
generateRows 1 = [[]]
generateRows n = [a:l | a <- colours, l <- generateRows (n-1)]

-- Labellings
data Label = One | Two
	deriving (Show, Eq)
labels = [One, Two]
type Labelling = [Label]

generateLabellings :: Int -> [Labelling]
generateLabellings 0 = [[]]
generateLabellings n = [a:l | a <- labels, l <- generateLabellings (n-1)]

--- Properties of RBGraphs

isCanonical :: RBGraph -> Bool
isCanonical g = and comps
	where 
		degs = map (redDegree g) [1..size g]
		comps = map compPair [1..size g - 1]
		
		compPair i = 
			if deg1 == deg2
				then
					if length (diff) == 0
						then True
						else Red == (fst $ head $ diff)
				else
					deg1 > deg2
			where
				deg1 = degs !! (i-1)
				deg2 = degs !! i	
				pairs = zip (edgeColours g i) (edgeColours g (i+1))
				diff  = filter (\(a,b) -> a /= b) pairs

isLocalOptima :: RBGraph -> Bool
isLocalOptima g = maximum rdegs <= (size g - 1) `div` 2
	where rdegs = map (redDegree g) [1..size g]
				
redDegree :: RBGraph -> Int -> Int
redDegree g i = length $ filter (==Red) $ edgeColours g i
	
edgeColours :: RBGraph -> Int -> [Colour]
edgeColours g i = prefix ++ suffix
	where
		rows = take (i-1) g
		prefix = zipWith (!!) rows $ reverse [0..i-2]
		suffix = if i == size g then [] else g !! (i-1)

interestGraphs :: Int -> [RBGraph]
interestGraphs n = [g | g <- generateRBs n, isCanonical g, isLocalOptima g]

canonicalGraphs :: Int -> [RBGraph]
canonicalGraphs n = [g | g <- generateRBs n, isCanonical g]

---- Code to search for bad triangles
	
findBadEdge :: RBGraph -> Maybe Edge
findBadEdge g = 
	if length (bad) == 0
		then Nothing
		else Just $ head (bad)
	where
		bad = filter (isBadEdge g) (edges g)
		
isBadEdge :: RBGraph -> Edge -> Bool
isBadEdge g e = (nBadEdge g e) > (size g - 2) `div` 2
			
nBadEdge :: RBGraph -> Edge -> Int
nBadEdge g e = length $ filter (isBadTriangle g e) (vertices g)

isBadTriangle :: RBGraph -> Edge -> Vertex -> Bool
isBadTriangle g e v | v == fst e || v == snd e = False
isBadTriangle g (i,j) k = length reds `mod` 2 == 1
	where reds = filter (isRedEdge g) [(i,j), (i,k), (j,k)]
	
isRedEdge :: RBGraph -> Edge -> Bool
isRedEdge g (i,j) | i > j = isRedEdge g (j,i)
isRedEdge g (i,j) = g !! (i-1) !! (j-i-1) == Red

--- vertex choosing algorithms
type VertAlg = RBGraph -> Vertex -> Labelling

vertAlgCost :: VertAlg -> RBGraph -> Int
vertAlgCost alg g = minimum costs
	where 
		costs = [cost g $ alg g $ v | v <- vertices g]

vertAlgRatio :: VertAlg -> RBGraph -> (Int, Int)
vertAlgRatio alg g = (vertAlgCost alg g, optCost g)


--- Code to test 3-approximation
-- pick a vertex
past_soln :: RBGraph -> Vertex -> Labelling
past_soln g v = colours2lbls v $ edgeColours g v

colours2lbls :: Vertex -> [Colour] -> Labelling
colours2lbls v colours = prefix ++ [One] ++ suffix
	where
		c2l Red = Two
		c2l Blue = One
		prefix = map c2l $ take (v-1) colours
		suffix = map c2l $ drop (v-1) colours

past_cost = vertAlgCost past_soln
past_ratio = vertAlgRatio past_soln
		
--- Code to test our improvements to PAV
pasta_flip_soln :: RBGraph -> Vertex -> Labelling
pasta_flip_soln g v = colours2lbls v $ edgeColours (search g) v
	where 
		verts   = vertices g
		edges   = [(v,i) | i <- verts, i /= v]
		minBad  = (size g - 2) `div` 2
		
		search g' | nBadEdge g' e <= minBad = g'
						  | otherwise = search (flipColours e g')
			where
				e = maxBy (nBadEdge g') edges
	
pasta_flip_cost = vertAlgCost pasta_flip_soln
pasta_flip_ratio = vertAlgRatio pasta_flip_soln
	
flipColours :: Edge -> RBGraph -> RBGraph			
flipColours (i, j) g | i > j = flipColours (j, i) g
flipColours (i, j) g = prefix ++ [newRow] ++ suffix
	where
		(prefix, (oldRow:suffix)) = splitAt (i-1) g
		
		newRow = rowPre ++ [flipColour c] ++ rowSuff
		(rowPre, (c:rowSuff)) = splitAt (j-i-1) oldRow
		
maxBy :: Ord b => (a -> b) -> [a] -> a
maxBy f [] = error "no items"
maxBy f (x:xs) = fst $ foldr comb (x, f x) xs
	where comb e (b, b_n) = if f e > b_n then (e, f e) else (b, b_n)
		
		
flipColour Red  = Blue
flipColour Blue = Red

optCost :: RBGraph -> Int
optCost g = minimum costs
	where
		lblings = generateLabellings (size g)
		costs = map (cost g) lblings
		
big = 100000000 :: Float
localRatio :: RBGraph -> Float
localRatio g | optCost g /= 0 = (fromIntegral worst) / (fromIntegral (optCost g))
             | worst > 0      = big
             | otherwise      = 1
	where worst = maximum [cost g l | l <- generateLabellings (size g), localMin g l]

-- is a labelling a local minimum?
localMin g l = maximum mistakes <= max_mist
	where
		mistakes = map miss $ vertices g
		max_mist = (size g - 1) `div` 2
		
		miss v = length $ [i | i <- vertices g, i /= v, not $ consist i]
			where
				consist i | isRedEdge g (i,v) = l !! (i-1) /= l !! (v-1)
				          | otherwise         = l !! (i-1) == l !! (v-1)

cost :: RBGraph -> Labelling -> Int
cost g lbls = length [e | e <- edges g, isCorrect e]
	where 
		isCorrect (i,j) | isRedEdge g (i,j) = lbli == lblj
		 								| otherwise = lbli /= lblj
			where 
				lbli = lbls !! (i-1)
				lblj = lbls !! (j-1)

main = do print out
	where
		graphs = interestGraphs 8
		out = map pasta_flip_ratio graphs
	
			 