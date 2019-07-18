#!/usr/bin/python

def f (n):
  sum = 0
  for i  in range (1, n):
    sum += i / (2.0**i)

  return sum
