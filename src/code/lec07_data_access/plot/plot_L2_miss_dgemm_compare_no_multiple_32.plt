set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./png/L2_miss_dgemm_compare_no_multiple_32.png"

set xlabel "M=N=K"
set ylabel "L2 Miss Rate"

set xrange [128:4096]
set yrange [0:]
set key right

plot \
'./data/dgemm_naive_no_multiple_32.dat' using 1:4 with linespoints lw 1 title "no blocking", \
'./data/dgemm_recursively_blocked_32_no_multiple_256.dat' using 1:4 with linespoints lw 1 title "recursive blocking", \