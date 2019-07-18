module Main (main) where
import RandomList
import Generate
import SearchTree
import Tournaments

--main = do f <- randomList
--	  print $ fst $ generateTourn 5 0.5 f

main = do --print $ getDataFromTourn $ evenTourn 4
          print $ as $ buildProbTree $ oddTourn 4
          --print $ getDataFromTourn $ badTourn 4
