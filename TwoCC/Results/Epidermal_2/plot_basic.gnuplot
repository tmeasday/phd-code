set term postscript eps solid 'Times-Bold' 14
set output 'plot.eps'
set size 1.1,0.7
set xlabel 'Time (in seconds)'
set ylabel 'Average number of errors'
set logscale x
plot '-' notitle pt 9, '-' with labels offset 5,0.5 notitle
54.01 198.0 DasGupta
0.4199 254.0 Spectral
1.37 249.0 PAST
3.55 200.0 PASTA-flip
2.22 208.0 PASTA-toss
4.25 198.0 PASTA-flip+toss
2.42 330.0 PTAS-10
79.06 325.0 PTAS-15
1.69 213.0 Tosses
e
44.01 200.0 DasGupta
0.4199 254.0 Spectral
1.37 249.0 PAST
3.55 200.0 PASTA-flip
2.22 207.5 PASTA-toss
5.25 190.0 PASTA-flip+toss
2.42 330.0 PTAS-10
35.06 325.0 PTAS-15
1.39 214.5 Tosses
e
