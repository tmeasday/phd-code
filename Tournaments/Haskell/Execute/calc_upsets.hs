import List
import Upsets
import Tournaments
import Order
import Util

main = 
  catch (do
    tournS <- getLine 
    main' tournS)
  (\_ -> return ())

  where 
  main' tournS = do 
    ordS <- getLine
    if (ordS == "")
      then do
        putStrLn ""
        main
      else do
        putStr $ show $ upsets (WT $ read tournS) (permute $ read ordS)
        putStr "\t"
        main' tournS

