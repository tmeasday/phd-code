module LocalSearch where

hillClimb :: Ord c => (b -> [b]) -> (b -> c) -> b -> b
hillClimb neigh cost state = case children neigh cost state of
				[]     -> state
				(s:ss) -> hillClimb neigh cost s

goWithTheWinners :: Ord c => Int -> (b -> [b]) -> (b -> c) -> b -> b
goWithTheWinners n neigh cost state = head $  gww (replicate n state) 
  where gww states | length alive == 0 = states
                   | otherwise         = gww newstates
          where (alive, dead) = span isAlive states
                isAlive s = length (children neigh cost s) > 0
                copies = 1 + (length dead) `div` (length alive)
                remainder = (length dead) `mod` (length alive)
                distribute s i | i < remainder = replicate (copies + 1) s
                               | otherwise     = replicate copies       s
                -- obviously this isn't actually random
                randomize s = head $ children neigh cost s

                newstates = [randomize s' | (s,i) <- zip alive [1..],
                                            s' <- distribute s i]



children :: Ord c => (b -> [b]) -> (b -> c) -> b -> [b]
children neigh cost state = [a | a <- neigh state, cost a < cost state]

