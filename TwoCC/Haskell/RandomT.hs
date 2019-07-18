module RandomT where

import Random
import Control.Monad.State

class (Monad m) => MonadRandom m where
  getRandom :: (Random a) => m a

newtype (RandomGen g) => RandT g m a = RandT (StateT g m a)
  deriving (Functor, Monad, MonadTrans, MonadIO)

liftState :: (MonadState s m) => (s -> (a,s)) -> m a
liftState t = do v <- get
                 let (x, v') = t v
                 put v'
                 return x

instance (Monad m, RandomGen g) => MonadRandom (RandT g m) where
  getRandom = RandT . liftState $ random
  
evalRandT :: (Monad m, RandomGen g) => RandT g m a -> g -> m a
evalRandT (RandT x) g = evalStateT x g

runRandT :: (Monad m, RandomGen g) => RandT g m a -> g -> m (a, g)
runRandT (RandT x) g = runStateT x g