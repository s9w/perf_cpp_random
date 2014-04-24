cpp: perf_cpp-random.cpp
	g++ -lboost_random -O3 -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random

debug: perf_cpp-random.cpp
	g++ -g -Wall -march=native -std=c++11 perf_cpp-random.cpp -o perf_cpp-random
