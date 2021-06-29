#include "Test.hpp"

std::string make_path(const int level, const bool adv, const int64_t iterations)
{
	std::stringstream ss;
	ss << "./output/" << level << '-' << (adv ? "adv" : "nonadv") << '-' << iterations << ".csv";
	// ss << level << '-' << (adv ? "adv" : "nonadv") << '-' << iterations << ".csv";
	return ss.str();
}

void test_flame()
{
	const Player doov(1.0/9.0, 2.8, 9.5);
	Flame f;
	f.add(FlameType::STR, 77);
	f.add(FlameType::DEX, 66);
	f.add(FlameType::INT, 55);
	f.add(FlameType::AS, 6);
	f.print_nonweapon_effects();
	std::cout << "score: " << f.score(FlameCriteria::STR, doov) << '\n';
}

void test_expected_value()
{
	const int level {200};
	Nonweapon equip(level);
	Player doov(0.11, 2.7, 9.5);
	constexpr int64_t iterations {2'000'000'000};
	constexpr int threads {32};
	auto h = FlameEngine::make_histogram(equip, doov, FlameMethod::ETERNAL, FlameCriteria::STR, iterations, threads);
	int score_to_beat {180}; // do bounds checking
	std::cout << "accumulation: " << std::accumulate(h.cbegin()+score_to_beat, h.cend(), 0) << '\n';
	std::cout << static_cast<double>(iterations)/std::accumulate(h.cbegin()+score_to_beat, h.cend(), 0) << '\n';
}

void gollux()
{
	const int level {150};
	Nonweapon equip(level, false);
	Player doov(0.11, 2.7, 9.5);
	constexpr int64_t iterations {2'000'000'000};
	constexpr int threads {32};
	auto h = FlameEngine::make_histogram(equip, doov, FlameMethod::ETERNAL, FlameCriteria::STR, iterations, threads);
	const std::string path = make_path(level, equip.has_advantage(), iterations);
	csv(h.cbegin(), h.cend(), path, iterations);
}

void sweetwater()
{
	const int level {160};
	Nonweapon equip(level, false);
	Player doov(0.11, 2.7, 9.5);
	constexpr int64_t iterations {10'000'000'000};
	constexpr int threads {32};
	auto h = FlameEngine::make_histogram(equip, doov, FlameMethod::ETERNAL, FlameCriteria::STR, iterations, threads);
	const std::string path = make_path(level, equip.has_advantage(), iterations);
	csv(h.cbegin(), h.cend(), path, iterations);
}

void arcane()
{
	const int level {200};
	Nonweapon equip(level);
	Player doov(0.11, 2.7, 9.5);
	constexpr int64_t iterations {320'000'000};
	constexpr int threads {32};
	auto h = FlameEngine::make_histogram(equip, doov, FlameMethod::ETERNAL, FlameCriteria::STR, iterations, threads);
	const std::string path = make_path(level, equip.has_advantage(), iterations);
	csv(h.cbegin(), h.cend(), path, iterations);
}

void absolab()
{
	const int level {160};
	Nonweapon equip(level);
	Player doov(0.11, 2.7, 9.5);
	constexpr int64_t iterations {320'000'000};
	constexpr int threads {32};
	auto h = FlameEngine::make_histogram(equip, doov, FlameMethod::ETERNAL, FlameCriteria::STR, iterations, threads);
	const std::string path = make_path(level, equip.has_advantage(), iterations);
	csv(h.cbegin(), h.cend(), path, iterations);
}
