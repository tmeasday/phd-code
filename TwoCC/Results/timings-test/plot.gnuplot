set term postscript eps 'Times-Bold' 14
set output 'plot.eps'
set size 1.1,0.7
set key left

#print "set term png"
#print "set output 'plot.png'"

set xlabel 'Problem size'
set ylabel 'Running time (in seconds)'


plot '-' title 'PAST' w l ls 1, '-' title 'PASTA-toss' w l ls 2, '-' title 'LocalSearch' w l ls 3, '-' title 'PASTA-flip' w l ls 4, '-' title 'PASTA-flip+toss' w l ls 5, '-' title 'PTAS-10' w l ls 6
10 0.01
20 0.06
30 0.18
40 0.39
50 0.76
75 2.47
100 5.72 
e
10 0.01
20 0.1
30 0.3
40 0.7
50 1.36
75 4.4
100 10.6 
e
10 0.01
20 0.07
30 0.25
40 0.56
50 1.08
75 3.51
100 8.62 
e
10 0.03
20 0.21
30 0.71
40 1.7
50 3.21
75 11.59
100 25.66 
e
10 0.03
20 0.24
30 0.79
40 1.89
50 3.61
75 12.8
100 28.99
e
10 0.36
20 1.96
30 4.28
40 7.21
50 10.85
75 22.85
100 39.22
e
