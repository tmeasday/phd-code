#!/usr/bin/python
import random

random.seed ()
for i in range (50):
  print "%.1f\t%.1f" % (random.random (), random.random ())
