gnuplot <<EOF
# Bloc 1 : Histogramme des 50 plus petites usines (Capacité maximale)
set terminal png size 1800,1000
set output "vol_max_small.png"
set title "50 Plus Petites Usines - Capacité Maximale"
set xlabel "Identifiants des Usines"
set ylabel "Capacité Maximale (M.m³)"
set grid
set style data histograms
set style fill solid border -1
set boxwidth 2 relative
set xtics offset 0, 0 font ",8"
set key autotitle columnhead
set datafile separator ";"
plot "vol_max.dat" using 2:xtic(1) title "Capacité" with histograms every ::1::50

# Bloc 2 : Histogramme des 10 plus grandes usines (Capacité maximale)
set output "vol_max_large.png"
set title "10 Plus Grandes Usines - Capacité Maximale"
plot "vol_max.dat" using 2:xtic(1) title "Capacité" with histograms every ::1::10

# Bloc 3 : Histogramme des 50 plus petites usines (Volume capté)
set output "vol_captation_small.png"
set title "50 Plus Petites Usines - Volume Capté"
set ylabel "Volume Capté (M.m³)"
plot "vol_captation.txt" using 2:xtic(1) title "Captation" with histograms every ::1::50

# Bloc 4 : Histogramme des 10 plus grandes usines (Volume capté)
set output "vol_captation_large.png"
set title "10 Plus Grandes Usines - Volume Capté"
plot "vol_captation.txt" using 2:xtic(1) title "Captation" with histograms every ::1::10

# Bloc 5 : Histogramme des 50 plus petites usines (Volume traité)
set output "vol_traitement_small.png"
set title "50 Plus Petites Usines - Volume Traité"
set ylabel "Volume Traité (M.m³)"
plot "vol_traitement.tmp" using 2:xtic(1) title "Traitement" with histograms every ::1::50

# Bloc 6 : Histogramme des 10 plus grandes usines (Volume traité)
set output "vol_traitement_large.png"
set title "10 Plus Grandes Usines - Volume Traité"
plot "vol_traitement.tmp" using 2:xtic(1) title "Traitement" with histograms every ::1::10
EOF