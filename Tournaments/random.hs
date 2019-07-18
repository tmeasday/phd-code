import Random

randomList :: IO [Int]
randomList = do g <- getStdGen
                return $ randoms g

main = do l <- randomList
          print l
