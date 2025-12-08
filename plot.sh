gnuplot <<EOF
set terminal png size 1800,1000
set output "output.png"
set title "Plant data (10 greatest)"
set xlabel "Plant IDs"
set ylabel "Volume (M.m³)"
set grid
set style data histograms
set style fill solid border -1
set boxwidth 2 relative
set xtics offset 0, 0 font ",8"
set key autotitle columnhead
set datafile separator ";"
plot "data/c-wildwater_v0.dat" using 4:xtic(2) title "Volume" with histograms
EOF