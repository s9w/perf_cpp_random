cpp: perf_cpp-random.cpp
	g++ -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random

debug: perf_cpp-random.cpp
	g++ -g -Wall -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random
<<<<<<< HEAD

build: gcc clang icc

gcc: perf_cpp-random.cpp
	g++ -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_gcc

clang: perf_cpp-random.cpp
	clang++ -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_clang

icc: perf_cpp-random.cpp
	icpc -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_icc

numbers_gcc: perf_cpp-random_gcc
	./perf_cpp-random gcc

numbers_clang: perf_cpp-random_clang
	./perf_cpp-random clang

numbers_icc: perf_cpp-random_icc
	./perf_cpp-random icc

numbers: numbers_icc numbers_gcc numbers_clang

plot: plot.plt
	gnuplot plot.plt
=======
>>>>>>> 1919eab49757eb8e19a6601cf5de3f6443e16d97
