gnuplot <<EOF
# Bloc 1 : Histogramme des 10 plus gros volumes
set terminal png size 1800,1000
set output "output_volume.png"
set title "Top 10 Plants by Volume"
set xlabel "Plant IDs"
set ylabel "Volume (M.m³)"
set grid
set style data histograms
set style fill solid border -1
set boxwidth 2 relative
set xtics offset 0, 0 font ",8"
set key autotitle columnhead
set datafile separator ";"
plot "vol_max.dat" using 2:xtic(1) title "Volume" with histograms

# Bloc 2 : Histogramme des 10 plus grandes fuites
set output "output_fuites.png"
set title "les fuites en aval des usines"
set ylabel "Leakage (%)"
plot "fuites_max.dat" using 2:xtic(1) title "Leakage" with histograms

# Bloc 3 : Histogramme personnalisé (par exemple, consommation)
set output "output_custom.png"
set title "Custom Histogram"
set ylabel "Custom Metric"
plot "src_data.dat" using 2:xtic(1) title "Custom" with histograms
EOF