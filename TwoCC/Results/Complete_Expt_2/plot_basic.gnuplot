set term postscript eps solid 'Times-Bold' 16
set output 'plot.eps'
set size 1.1,0.7
set xlabel 'Time (in seconds)'
set ylabel 'Average number of errors'
set logscale x
set yrange [2080:]
plot '-' notitle pt 9, '-' with labels offset 5,0.5 notitle
689.008 2119.8 DasGupta
114.3521 2150.1 Spectral
4.54 2339.84 PAST
20.77 2106.7 PASTA-flip
8.54 2106.07 PASTA-toss
23.18 2106.14 PASTA-flip+toss
31.45 2259.11 PTAS-10
1052.27 2216.74 PTAS-15
6.91 2106.61 LocalSearch
e
689.008 2119.8 DasGupta
114.3521 2150.1 Spectral
4.54 2319.84 PAST
15.77 2110.7 PASTA-flip
4.54 2110.07 PASTA-toss
25.18 2090.14 PASTA-flip+toss
31.45 2259.11 PTAS-10
1052.27 2216.74 PTAS-15
2.3 2090.61 Tosses
e
