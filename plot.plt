set term png
set datafile separator "|"
set key top left
set style data linespoints
set style data histogram
set style fill solid border -1
set xtics rotate by -25
set xtics font "3"
set ylabel "runtime [ms]"
set grid y

set output "plot_calls.png"
plot [][0:] 'output_rng_calls_gcc.txt' using 3:xticlabels(1) title "gcc" ,\
	'output_rng_calls_clang.txt' using 3:xticlabels(1) title "clang" ,\
	'output_rng_calls_icc.txt' using 3:xticlabels(1) title "icc"

set output "plot_int.png"
plot [][0:] 'output_rng_int_gcc.txt' using 2:xticlabels(1) title "gcc" ,\
	'output_rng_int_clang.txt' using 2:xticlabels(1) title "clang" ,\
	'output_rng_int_icc.txt' using 2:xticlabels(1) title "icc"