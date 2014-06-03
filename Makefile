debug: perf_cpp-random.cpp
	g++ -g -Wall -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random

build: gcc clang icc

gcc: perf_cpp-random.cpp
	g++ -lboost_random -O3 -march=native -finline-limit=10000 -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_gcc

asm: perf_cpp-random.cpp
	g++ -lboost_random -O3 -march=native -S -fno-asynchronous-unwind-tables -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_gcc

clang: perf_cpp-random.cpp
	clang++ -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_clang

icc: perf_cpp-random.cpp
	icpc -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random_icc

numbers_gcc: perf_cpp-random_gcc
	./perf_cpp-random_gcc gcc

numbers_clang: perf_cpp-random_clang
	./perf_cpp-random_clang clang

numbers_icc: perf_cpp-random_icc
	./perf_cpp-random_icc icc

numbers: numbers_gcc numbers_clang numbers_icc

plot: plot.plt
	gnuplot plot.plt
