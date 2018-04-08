#!/usr/bin/gnuplot
# plot results of conv_base.acc_test program

plot "conv_base.acc_test1.tmp" using ($1/10):($2/10) with linespoints pt 7,\
     "conv_base.acc_test2.tmp" using 1:2 with linespoints pt 7

pause -1
