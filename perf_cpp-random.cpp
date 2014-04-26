#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <limits>
#include <boost/random.hpp>

double timer_bits1(int n, std::mt19937_64 &rng) {
	std::vector<int> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = rng() % 2;
	auto t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; // access to avoid compiler skipping
	return runtime;
}

double timer_bits2(int n, std::mt19937_64 &rng) {
	unsigned long random_ulong = rng();
	int shifts = 0;
	std::vector<int> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i) {
		if (shifts >= 63) {
			random_ulong = rng();
			shifts = 0;
		}
		vec[i] = (random_ulong >> shifts) & 1;
		shifts++;
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; // access to avoid compiler skipping
	return runtime;
}

template <typename T_rng, typename T_dist>
double timer_rng(T_rng &rng, T_dist &dist, int n) {
	std::vector<typename T_dist::result_type> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = dist(rng);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; //access to avoid compiler skipping
	return runtime;
}

template <typename T_rng>
double timer_rng_modulo(T_rng &rng, int n) {
	std::vector<int> vec(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec[i] = rng();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec[0]; //access to avoid compiler skipping
	return runtime;
}

int main() {
	const int n = 10000000;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rng_default(seed);
	std::mt19937 rng_mt(seed);
	std::mt19937_64 rng_mt_64(seed);
	std::minstd_rand rng_minstd(seed);
	std::random_device rng_urandom("/dev/urandom");
	std::random_device rng_rdrand;
	std::uniform_int_distribution<unsigned long> dist_int(0, 1000);
	std::uniform_real_distribution<float> dist_float(0.0, 1.0);
	std::uniform_real_distribution<double> dist_double(0.0, 1.0);
	boost::random::mt19937 rng_boost_mt19937;
	boost::random::mt19937_64 rng_boost_mt19937_64;
	boost::random::uniform_int_distribution<> dist_boost_int(1, 10000);

	// max values
	std::cout << "max values" << std::endl;
	std::cout << "rand:() " << RAND_MAX << std::endl;
	std::cout << "default_random_engine: " << rng_default.max() << std::endl;
	std::cout << "mt19937: " << rng_mt.max() << std::endl;
	std::cout << "mt19937_64: " << rng_mt_64.max() << std::endl;
	std::cout << "rng_boost_mt19937: " << rng_boost_mt19937.max() << std::endl;
	std::cout << "rng_boost_mt19937_64: " << rng_boost_mt19937_64.max() << std::endl;
	std::cout << "minstd_rand: " << rng_minstd.max() << std::endl;
	std::cout << "rng_urandom: " << rng_urandom.max() << std::endl;
	std::cout << "rng_rdrand: " << rng_rdrand.max() << std::endl;

	// baseline uint
	std::vector<uint_fast64_t> vec_uint(n, 0);
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec_uint[i] = 4;
	auto t2 = std::chrono::high_resolution_clock::now();
	auto runtime_uint_baseline = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	auto sum = vec_uint[0];

	// baseline int
	std::vector<int> vec_int(n, 0);
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec_int[i] = 4;
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime_int_baseline = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	sum = vec_int[0];

	// time rand()
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; ++i)
		vec_int[i] = rand();
	t2 = std::chrono::high_resolution_clock::now();
	auto runtime_int_rand = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.0;
	sum = vec_int[0];

	// Integers
	std::cout << std::endl << "runtimes int" << std::endl;
	std::cout << "baseline int: " << runtime_int_baseline << std::endl;
	std::cout << "baseline uint_fast64_t: " << runtime_uint_baseline << std::endl;
	std::cout << "rand: " << runtime_int_rand << std::endl;
	std::cout << "rng_default: " << timer_rng(rng_default, dist_int, n) << std::endl;
	std::cout << "rng_mt: " << timer_rng(rng_mt, dist_int, n) << std::endl;
	std::cout << "rng_mt_modulo: " << timer_rng_modulo(rng_mt, n) << std::endl;
	std::cout << "rng_mt_64: " << timer_rng(rng_mt_64, dist_int, n) << std::endl;
	std::cout << "rng_minstd: " << timer_rng(rng_minstd, dist_int, n) << std::endl;
	std::cout << "rng_urandom: " << timer_rng(rng_urandom, dist_int, n) << std::endl;
	std::cout << "rng_rdrand: " << timer_rng(rng_rdrand, dist_int, n) << std::endl;
	std::cout << "rng_boost_mt19937: " << timer_rng(rng_boost_mt19937, dist_boost_int, n) << std::endl;
	std::cout << "rng_boost_mt19937_64: " << timer_rng(rng_boost_mt19937_64, dist_boost_int, n) << std::endl;

	// Bits
	int n2 = 10000000;
	std::cout << std::endl;
	std::cout << "runtime bits 1: " << timer_bits1(n2, rng_mt_64) << std::endl;
	std::cout << "runtime bits 2: " << timer_bits2(n2, rng_mt_64) << std::endl;

	// floats and doubles
	std::cout << std::endl << "runtimes float" << std::endl;
	std::cout << "rng_default: " << timer_rng(rng_default, dist_float, n) << std::endl;
	std::cout << "rng_mt: " << timer_rng(rng_mt, dist_float, n) << std::endl;
	std::cout << "rng_mt_64: " << timer_rng(rng_mt_64, dist_float, n) << std::endl;
	std::cout << "rng_minstd: " << timer_rng(rng_minstd, dist_float, n) << std::endl;
	std::cout << "rng_boost_mt19937: " << timer_rng(rng_boost_mt19937, dist_float, n) << std::endl;
	std::cout << "rng_boost_mt19937_64: " << timer_rng(rng_boost_mt19937_64, dist_float, n) << std::endl;
	std::cout << "rng_urandom: " << timer_rng(rng_urandom, dist_float, n) << std::endl;
	std::cout << "rng_rdrand: " << timer_rng(rng_rdrand, dist_float, n) << std::endl;

	std::cout << std::endl << "runtimes double" << std::endl;
	std::cout << "rng_default: " << timer_rng(rng_default, dist_double, n) << std::endl;
	std::cout << "rng_mt: " << timer_rng(rng_mt, dist_double, n) << std::endl;
	std::cout << "rng_mt_64: " << timer_rng(rng_mt_64, dist_double, n) << std::endl;
	std::cout << "rng_minstd: " << timer_rng(rng_minstd, dist_double, n) << std::endl;
	std::cout << "rng_boost_mt19937: " << timer_rng(rng_boost_mt19937, dist_double, n) << std::endl;
	std::cout << "rng_boost_mt19937_64: " << timer_rng(rng_boost_mt19937_64, dist_double, n) << std::endl;
	std::cout << "rng_urandom: " << timer_rng(rng_urandom, dist_double, n) << std::endl;
	std::cout << "rng_rdrand: " << timer_rng(rng_rdrand, dist_double, n) << std::endl;

	// data type limits
	// std::cout << "std::numeric_limits<int>::max(): " << std::numeric_limits<uint_fast64_t>::max() << std::endl;
}