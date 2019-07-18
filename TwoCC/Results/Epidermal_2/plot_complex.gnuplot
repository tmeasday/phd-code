set term postscript eps solid 'Times-Bold' 14
set output 'plot.eps'
set size 1.1,0.7
set xlabel 'Time (in seconds)'
set ylabel 'Average number of errors'
set logscale x
plot '-' notitle pt 9, '-' with labels offset 5,0.5 notitle
3.55 200.0 PASTA-flip
2.22 208.0 PASTA-toss
4.25 198.0 PASTA-flip+toss
24.63 206.0 PASTA-toss-rand
25.97 202.0 PASTA-flip-rand
0.06 204.0 PASTA-flip-cover
0.04 230.0 PASTA-toss-cover
0.13 203.0 PASTA-flip-10
0.3 201.0 PASTA-flip-25
0.58 201.0 PASTA-flip-50
0.08 210.0 PASTA-toss-10
0.18 208.0 PASTA-toss-25
0.36 208.0 PASTA-toss-50
e
3.55 200.0 PASTA-flip
2.22 208.0 PASTA-toss
4.25 196.0 PASTA-flip+toss
24.63 206.0 PASTA-toss-rand
25.97 202.0 PASTA-flip-rand
0.06 204.5 PASTA-flip-cover
0.04 228.0 PASTA-toss-cover
0.13 203.0 PASTA-flip-10
0.2 201.2 PASTA-flip-25
0.58 199.0 PASTA-flip-50
0.08 210.5 PASTA-toss-10
0.15 208.3 PASTA-toss-25
0.36 206.0 PASTA-toss-50
e
