module Generate where
import Tournaments

-- Generate Tournaments randomly

-- no messing around here. 
-- assume we bias toward [0..n]
-- generate an infinite list of tournaments of size n
-- with prob. p of edges being 'correct'
generateTourn :: Int -> Float -> [Float] -> (Tournament, [Float])
generateTourn 0 _ rands = (T [], rands)
generateTourn n p rands = (T (row:rows), rands'')
	where (row, rands')     = generateRow n p rands
	      (T rows, rands'') = generateTourn (n-1) p rands'

generateRow :: Int -> Float -> [Float] -> ([Bool], [Float])
generateRow 0 _ rs = ([], rs)
generateRow n p (r:rs) | p > r     = (True : bs, rs')
		       | otherwise = (False: bs, rs')
	where (bs, rs') = generateRow (n-1) p rs
