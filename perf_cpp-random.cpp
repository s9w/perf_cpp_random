#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <limits>
#include <boost/random.hpp>
#include <fstream>

double timer_bits1(int n, std::mt19937_64 &rng) {
	std::vector<int> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = rng() % 2;
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; // access to avoid compiler skipping
	return runtime;
}

double timer_bits2(int n, std::mt19937_64 &rng) {
	unsigned long random_ulong = rng();
	int shifts = 0;
	std::vector<int> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	 t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i) {
		if (shifts >= 63) {
			random_ulong = rng();
		          	shifts = 0;
		}
		vec[i] = (random_ulong >> shifts) & 1;
		shifts++;
	}
	 t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; // access to avoid compiler skipping
	return runtime;
}

template <typename T_rng, typename T_dist>
double timer_rng_dist(T_rng &rng, T_dist &dist, int n) {
	std::vector<typename T_dist::result_type> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = dist(rng);
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; //access to avoid compiler skipping
	return runtime;
}

template <typename T_rng>
double timer_rng_mod(T_rng &rng, int n) {
	std::vector<typename T_rng::result_type> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = rng()%1024;
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; //access to avoid compiler skipping
	return runtime;
}

template <typename T_rng>
double timer_rng(T_rng &rng, int n) {
	std::vector<typename T_rng::result_type> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = rng();
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; //access to avoid compiler skipping
	return runtime;
}

// template <typename T_rng>
// double timer_rng_modulo(T_rng &rng, int n) {
// 	std::vector<typename T_rng::result_type> vec(n, 0);
// 	auto t1 = std::chrono::high_resolution_clock::now();
// 	auto t2 = std::chrono::high_resolution_clock::now();
// 	t1 = std::chrono::high_resolution_clock::now();
// 	for (int i = 0; i < n; ++i)
// 		vec[i] = (rng() / rng.max())*1000;
// 	t2 = std::chrono::high_resolution_clock::now();
// 	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
// 	auto sum = vec[0]; //access to avoid compiler skipping
// 	return runtime;
// }

// template <typename T_rng>
// int uniform_int_distribution(T_rng &rng, int a, int b){
// 	return rng.max()
// }

std::string format_max(uint_fast64_t max){
	if(max == 4294967295 )
		return "2^32-1";
	else if(max == 2147483646 )
		return "2^31-2";
	else if(max == 2147483647 )
		return "2^31-1";
	else if(max == 18446744073709551615 )
		return "2^64-1";
	return std::to_string(max);
}

int main(int argc, char* argv[]) {
	std::string compiler = "default";
	if(argc>=2)
		compiler = argv[1];
	const int n = 10000000;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rng_default(seed);
	std::mt19937 rng_mt(seed);
	std::mt19937_64 rng_mt_64(seed);
	std::minstd_rand rng_minstd(seed);
	boost::random::mt19937 rng_boost_mt19937;
	boost::random::mt19937_64 rng_boost_mt19937_64;
	std::random_device rng_rdrand;
	std::random_device rng_urandom("/dev/urandom");
	std::uniform_int_distribution<int> dist_int(0, 1000);
	std::uniform_int_distribution<unsigned long> dist_ulong(0, 1000);
	std::uniform_real_distribution<float> dist_float(0.0, 1.0);
	std::uniform_real_distribution<double> dist_double(0.0, 1.0);
	boost::random::uniform_int_distribution<> dist_boost_int(1, 1000);
	std::cout << std::fixed << std::setprecision(2);

	// baseline int
	std::vector<int> vec_int(n, 0);
	std::vector<uint_fast64_t> vec_uint(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec_int[i] = 4;
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime_int_baseline = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec_int[0];

	// baseline uint
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec_uint[i] = 4;
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime_uint_baseline = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	sum = vec_uint[0];

	// time rand()
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec_int[i] = rand();
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime_rand = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	sum = vec_int[0];

	// RNG calls
	std::cout << "starting rng calls..." << std::endl;
	std::ofstream f_calls("output_rng_calls_"+compiler+".txt");
	f_calls << "# RNG calls" << std::endl;
	f_calls << "baseline_int | - | " << runtime_int_baseline << std::endl;
	f_calls << "baseline_uint_fast64_t | - | " << runtime_uint_baseline << std::endl;
	f_calls << "std::default_random_engine | " << format_max(rng_default.max()) << " | " << timer_rng(rng_default, n) << std::endl;
	f_calls << "std::mt19937 | " << format_max(rng_mt.max()) << " | " << timer_rng(rng_mt, n) << std::endl;
	f_calls << "std::mt19937_64 | " << format_max(rng_mt_64.max()) << " | " << timer_rng(rng_mt_64, n) << std::endl;
	f_calls << "std::minstd_rand | " << format_max(rng_minstd.max()) << " | " << timer_rng(rng_minstd, n) << std::endl;
	f_calls << "boost::random::mt19937 | " << format_max(rng_boost_mt19937.max()) << " | " << timer_rng(rng_boost_mt19937, n) << std::endl;
	f_calls << "boost::random::mt19937_64 | " << format_max(rng_boost_mt19937_64.max()) << " | " << timer_rng(rng_boost_mt19937_64, n) << std::endl;
	f_calls << "RdRand | " << format_max(rng_rdrand.max()) << " | " << timer_rng(rng_rdrand, n) << std::endl;
	f_calls << "/dev/urandom | " << format_max(rng_urandom.max()) << " | " << timer_rng(rng_urandom, n) << std::endl;
	f_calls << "rand() | " << format_max(RAND_MAX) << " | " << runtime_rand << std::endl;
	f_calls.close();

	// Integers
	std::cout << "starting integer calls..." << std::endl;
	std::ofstream f_int("output_rng_int_"+compiler+".txt");
	f_int << std::endl << "# runtimes int" << std::endl;
	f_int << "std::default_random_engine | " << timer_rng_dist(rng_default, dist_int, n) << " | " << timer_rng_mod(rng_default, n) << std::endl;
	f_int << "std::mt19937 | " << timer_rng_dist(rng_mt, dist_int, n) << " | " << timer_rng_mod(rng_mt, n) << std::endl;
	//f_intout << "rng_mt_modulo | " << timer_rng_modulo(rng_mt, n) << std::endl;
	f_int << "std::mt19937_64 | " << timer_rng_dist(rng_mt_64, dist_ulong, n) << " | " << timer_rng_mod(rng_mt_64, n) << std::endl;
	f_int << "std::minstd_rand | " << timer_rng_dist(rng_minstd, dist_int, n) << " | " << timer_rng_mod(rng_minstd, n) << std::endl;
	f_int << "boost::random::mt19937 | " << timer_rng_dist(rng_boost_mt19937, dist_boost_int, n) << " | " << timer_rng_mod(rng_boost_mt19937, n) << std::endl;
	f_int << "boost::random::mt19937_64 | " << timer_rng_dist(rng_boost_mt19937_64, dist_boost_int, n) << " | " << timer_rng_mod(rng_boost_mt19937_64, n) << std::endl;
	f_int << "RdRand | " << timer_rng_dist(rng_rdrand, dist_int, n) << " | " << timer_rng_mod(rng_rdrand, n) << std::endl;
	f_int << "/dev/urandom | " << timer_rng_dist(rng_urandom, dist_int, n) << " | " << timer_rng_mod(rng_urandom, n) << std::endl;
	f_int.close();

	// // Bits
	// int n2 = 10000000;
	// std::cout << std::endl;
	// std::cout << "runtime bits 1: " << timer_bits1(n2, rng_mt_64) << std::endl;
	// std::cout << "runtime bits 2: " << timer_bits2(n2, rng_mt_64) << std::endl;

	// // floats and doubles
	// std::cout << "starting float calls..." << std::endl;
	// std::cout << std::endl << "# runtimes float" << std::endl;
	// std::cout << "rng_default: " << timer_rng_dist(rng_default, dist_float, n) << std::endl;
	// std::cout << "rng_mt: " << timer_rng_dist(rng_mt, dist_float, n) << std::endl;
	// std::cout << "rng_mt_64: " << timer_rng_dist(rng_mt_64, dist_float, n) << std::endl;
	// std::cout << "rng_minstd: " << timer_rng_dist(rng_minstd, dist_float, n) << std::endl;
	// std::cout << "rng_boost_mt19937: " << timer_rng_dist(rng_boost_mt19937, dist_float, n) << std::endl;
	// std::cout << "rng_boost_mt19937_64: " << timer_rng_dist(rng_boost_mt19937_64, dist_float, n) << std::endl;
	// std::cout << "rng_urandom: " << timer_rng_dist(rng_urandom, dist_float, n) << std::endl;
	// std::cout << "rng_rdrand: " << timer_rng_dist(rng_rdrand, dist_float, n) << std::endl;

	// std::cout << std::endl << "runtimes double" << std::endl;
	// std::cout << "rng_default: " << timer_rng_dist(rng_default, dist_double, n) << std::endl;
	// std::cout << "rng_mt: " << timer_rng_dist(rng_mt, dist_double, n) << std::endl;
	// std::cout << "rng_mt_64: " << timer_rng_dist(rng_mt_64, dist_double, n) << std::endl;
	// std::cout << "rng_minstd: " << timer_rng_dist(rng_minstd, dist_double, n) << std::endl;
	// std::cout << "rng_boost_mt19937: " << timer_rng_dist(rng_boost_mt19937, dist_double, n) << std::endl;
	// std::cout << "rng_boost_mt19937_64: " << timer_rng_dist(rng_boost_mt19937_64, dist_double, n) << std::endl;
	// std::cout << "rng_urandom: " << timer_rng_dist(rng_urandom, dist_double, n) << std::endl;
	// std::cout << "rng_rdrand: " << timer_rng_dist(rng_rdrand, dist_double, n) << std::endl;

	// data type limits
	// std::cout << "std::numeric_limits<int>::max(): " << std::numeric_limits<uint_fast64_t>::max() << std::endl;
}