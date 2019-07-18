module RandomList where
--module RandomList (randomList, splitList, mapR) where
import Random

randomList :: IO [Float]
randomList = do g <- getStdGen
                return $ randoms g

splitList :: [a] -> ([a], [a])
splitList []  = ([], [])
splitList [a] = ([a], [])
splitList (a:b:cs) = (a:as, b:bs)
	where (as, bs) = splitList cs

mapR :: (a -> [b] -> c) -> [a] -> [b] -> [c]
mapR _ [] _ = []
mapR f (x:xs) list = (f x list' : mapR f xs list'')
  where (list', list'') = splitList list


-- TODO is it better to do something like
-- mapR :: (a -> [b] -> (c, [b])) -> [a] -> [b] -> ([c], [b])
-- mapR _ [] rands = ([], rands)
-- mapR f (x:xs) rands = (y : ys, rands'')
--    where y, rands'   = f x rands
--          ys, rands'' = mapR f xs rands'

