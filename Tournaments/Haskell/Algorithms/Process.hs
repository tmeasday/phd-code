module Process
where

import RandomList

process :: Read a => (a -> [Int]) -> IO ()
process order = 
  catch (do
    line <- getLine
    print $ order (read line)
    process order)
  (\e -> return ())

process_random :: Read a => (a -> [Float] -> [Int]) -> IO ()
process_random order = 
  do list <- randomList
     pr' order list
     return ()
  where 
    pr' order list = 
      catch (do
        line <- getLine
        let (list', list'') = splitList list
        print $ order (read line) list'
        pr' order list'')
      (\e -> return ())
