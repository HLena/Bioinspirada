1000 1000 fname "Times" fsize 36
set output "plot.svg"
set title "A simple plot of x^2 vs. x"
set xlabel "x"
set ylabel "y"
plot "./puntos.dat" using 1:2 title ""