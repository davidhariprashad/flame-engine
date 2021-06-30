#ifndef TEST_HPP
#define TEST_HPP

#include "FlameEngine.hpp"

#include <fstream>
#include <sstream>
#include <exception>

template <typename ForwardIt>
void csv(ForwardIt first, const ForwardIt last, const std::string path, const int64_t iterations)
{
	std::ofstream os(path.c_str());
	if (!os.is_open())
	{
		throw std::runtime_error("Unable to open file.");
	}
	std::cout << "Outputting to " << path << '\n';
	std::for_each(first, last, [&os, index = 0, sum = 0ULL, cdf = 0.0, iterations](const auto& ref) mutable
	{
		sum += ref;
		const double pdf = ref/static_cast<double>(iterations);
		cdf = sum/static_cast<double>(iterations);
		const double expected_value = 1/(1-cdf);
		// os << index++ << ',' << expected_value << ',' << cdf << ',' << pdf << ',' << ref << '\n';
		os << index++ << ',' << expected_value << '\n';
	});
}

std::string make_path(const int level, const bool adv, const int64_t iterations);
void test_flame();
void test_expected_value();
void gollux();
void sweetwater();
void arcane();
void absolab();
void cra();
void papulatus();

#endif
