#!/usr/bin/gnuplot

plot "conv_base.test.frw_acc1.tmp" using ($1*.1):($2*.1) with linespoints pt 7,\
     "conv_base.test.frw_acc2.tmp" using 1:2 with linespoints pt 7

pause -1
