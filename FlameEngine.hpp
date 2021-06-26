#ifndef FLAMEENGINE_HPP
#define FLAMEENGINE_HPP

#include "FlameTraits.hpp"
#include "Flame.hpp"
#include "Equip.hpp"
#include "Player.hpp"

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <thread>
#include <cstdint>

class FlameEngine
{

	static constexpr int FLAME_LIMIT {256};
	static constexpr int FD_LIMIT {10};
	static constexpr int FD_BINS {100};

public:

	typedef std::array<int64_t, FLAME_LIMIT> ScoreHistogram;
	typedef std::array<int64_t, FD_BINS> FDHistogram;
	typedef void (FlameEngine::*Worker)(const int64_t k);

	template <typename Key, typename ArrayType, std::size_t ArraySize, std::size_t TableSize,
		typename Array = std::array<ArrayType, ArraySize>,
		typename RowType = std::pair<Key, Array>>
	using ArrayLookupTable = std::array<RowType, TableSize>;

	template <typename InputIt, typename OutputIt>
	static void make_pool(InputIt first, InputIt last, OutputIt out, int init)
	{
		std::for_each(first, last, [out, init](auto& value) mutable
		{
			out = std::fill_n(out, value, init++);
		});
	}

	template <typename ArrayType, int Size, typename ForwardIt, typename Key, typename Comparator>
	static const std::array<ArrayType, Size>& lookup_array_ref(ForwardIt first, const ForwardIt last, const Key& key, const Comparator f)
	{
		const auto it = std::find_if(first, last, [key, &f](const auto& pair)
		{
			return f(key, pair.first);
		});
		if (it == last)
		{
			throw std::out_of_range("Out of range.");
		}
		return it->second;
	}

	template <typename ForwardIt>
	static void print(ForwardIt first, ForwardIt last, char c = ',')
	{
		std::for_each(first, last, [index = 0, c](const auto& ref) mutable
		{
			std::cout << index++ << c << ref << '\n';
		});
	}

	static constexpr ArrayLookupTable<int, int, 7, 14> str_dex_int_luk_def_table {{
		{19, {1, 2, 3, 4, 5, 6, 7}},
		{39, {2, 4, 6, 8, 10, 12, 14}},
		{59, {3, 6, 9, 12, 15, 18, 21}},
		{79, {4, 8, 12, 16, 20, 24, 28}},
		{99, {5, 10, 15, 20, 25, 30, 35}},
		{119, {6, 12, 18, 24, 30, 36, 42}},
		{139, {7, 14, 21, 28, 35, 42, 49}},
		{159, {8, 16, 24, 32, 40, 48, 56}},
		{179, {9, 18, 27, 36, 45, 54, 63}},
		{199, {10, 20, 30, 40, 50, 60, 70}},
		{219, {11, 22, 33, 44, 55, 66, 77}},
		{239, {12, 24, 36, 48, 60, 72, 84}},
		{259, {13, 26, 39, 52, 65, 78, 91}},
		{275, {14, 28, 42, 56, 70, 84, 98}}}};

	static constexpr ArrayLookupTable<int, int, 7, 7> pair_stat_table {{
		{39, {1, 2, 3, 4, 5, 6, 7}},
		{79, {2, 4, 6, 8, 10, 12, 14}},
		{119, {3, 6, 9, 12, 15, 18, 21}},
		{159, {4, 8, 12, 16, 20, 24, 28}},
		{199, {5, 10, 15, 20, 25, 30, 35}},
		{239, {6, 12, 18, 24, 30, 36, 42}},
		{275, {7, 14, 21, 28, 35, 42, 49}}}};

	static constexpr ArrayLookupTable<int, int, 7, 1> atk_matk_speed_jump_dmg_as_table {{
		{275, {1, 2, 3, 4, 5, 6, 7}}}};

	static constexpr ArrayLookupTable<int, int, 7, 1> bd_table {{
		{275, {2, 4, 6, 8, 10, 12, 14}}}};

	static constexpr ArrayLookupTable<int, double, 7, 7> nonadvantage_attack_ratio_table {{
		{39, {.01, .022, .0363, .0524, .073205, .087846, .102487}},
		{79, {.02, .044, .0726, .10648, .14641, .175692, .204974}},
		{119, {.03, .066, .1089, .15972, .219615, .263538, .307461}},
		{159, {.04, .088, .1452, .21296, .29282, .351384, .409948}},
		{199, {.05, .11, .1815, .2662, .366025, .43923, .512435}},
		{239, {.06, .132, .2178, .31944, .43923, .527076, .614922}},
		{275, {.07, .154, .2541, .37268, .512435, .614922, .717409}}}};

	static constexpr ArrayLookupTable<int, double, 7, 7> advantage_attack_ratio_table {{
		{39, {.0, .0, .03, .044, .0605, .07986, .102487}},
		{79, {.0, .0, .06, .088, .121, .15972, .204974}},
		{119, {.0, .0, .09, .132, .1815, .23958, .307461}},
		{159, {.0, .0, .12, .176, .242, .31944, .409948}},
		{199, {.0, .0, .15, .22, .3025, .3993, .512435}},
		{239, {.0, .0, .18, .264, .363, .47916, .614922}},
		{275, {.0, .0, .21, .308, .4235, .55902, .717409}}}};

	static constexpr ArrayLookupTable<int, int, 7, 28> hp_mp_table {{
		{9, {3, 6, 9, 12, 15, 18, 21}},
		{19, {30, 60, 90, 120, 150, 180, 210}},
		{29, {60, 120, 180, 240, 300, 360, 420}},
		{39, {90, 180, 270, 360, 450, 540, 630}},
		{49, {120, 240, 360, 480, 600, 720, 840}},
		{59, {150, 300, 450, 600, 750, 900, 1050}},
		{69, {180, 360, 540, 720, 900, 1080, 1260}},
		{79, {210, 420, 630, 840, 1050, 1260, 1470}},
		{89, {240, 480, 720, 960, 1200, 1440, 1680}},
		{99, {270, 540, 810, 1080, 1350, 1620, 1890}},
		{109, {300, 600, 900, 1200, 1500, 1800, 2100}},
		{119, {330, 660, 990, 1320, 1650, 1980, 2310}},
		{129, {360, 720, 1080, 1440, 1800, 2160, 2520}},
		{139, {390, 780, 1170, 1560, 1950, 2340, 2730}},
		{149, {420, 840, 1260, 1680, 2100, 2520, 2940}},
		{159, {450, 900, 1350, 1800, 2250, 2700, 3150}},
		{169, {480, 960, 1440, 1920, 2400, 2880, 3360}},
		{179, {510, 1020, 1530, 2040, 2550, 3060, 3570}},
		{189, {540, 1080, 1620, 2160, 2700, 3240, 3780}},
		{199, {570, 1140, 1710, 2280, 2850, 3420, 3990}},
		{209, {600, 1200, 1800, 2400, 3000, 3600, 4200}},
		{219, {630, 1260, 1890, 2520, 3150, 3780, 4410}},
		{229, {660, 1320, 1980, 2640, 3300, 3960, 4620}},
		{239, {690, 1380, 2070, 2760, 3450, 4140, 4830}},
		{249, {720, 1440, 2160, 2880, 3600, 4320, 5040}},
		{259, {750, 1500, 2250, 3000, 3750, 4500, 5250}},
		{269, {780, 1560, 2340, 3120, 3900, 4680, 5460}},
		{275, {810, 1620, 2430, 3240, 4050, 4860, 5670}}}};

	static constexpr ArrayLookupTable<int, int, 7, 1> lev_table {{
		{275, {5, 10, 15, 20, 25, 30, 35}}}};

	static constexpr ArrayLookupTable<FlameMethod, int, 5, FlameTraits::METHOD_COUNT> tier_percents_table {{
		{FlameMethod::DROP_NPC_SHARD, {25, 30, 30, 14, 1}},
		{FlameMethod::SHARD_CHANCE_TIME, {0, 30, 50, 19, 1}},
		{FlameMethod::POWERFUL, {20, 30, 36, 14, 0}},
		{FlameMethod::ETERNAL, {0, 29, 45, 25, 1}},
		{FlameMethod::CRAFTING_FUSING, {50, 40, 10, 0, 0}},
		{FlameMethod::MASTER_CRAFT, {15, 30, 40, 14, 1}},
		{FlameMethod::MASTER_FUSE, {25, 35, 30, 10, 0}},
		{FlameMethod::MEISTER_CRAFT, {0, 19, 50, 30, 1}},
		{FlameMethod::MEISTER_FUSE, {0, 40, 45, 14, 1}}}};

	static constexpr std::array<int, 4> nonadvantage_effect_counts = {
		45, 35, 15, 5};

	static ScoreHistogram make_histogram(const Nonweapon, const Player, const FlameMethod, const FlameCriteria, const int64_t n = 32'000'000, const int c = 32);

	int advantage_effects_count() const;
	int nonadvantage_effects_count();

private:

	FlameEngine(const Player& p, const FlameMethod m, const FlameCriteria c, const std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT>& table, const std::vector<int>& t_pool, const std::vector<int>& c_pool);

	std::thread thread;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> distribution;
	ScoreHistogram score_histogram;
	FDHistogram fd_histogram;
	Player player;
	FlameMethod method;
	FlameCriteria criteria;
	std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT> stat_table;
	std::vector<int> tier_pool;
	std::vector<int> effect_count_pool;
	std::array<FlameType, FlameTraits::TYPE_COUNT> effect_type_pool;
	std::vector<FlameType> types;
	std::vector<int> tiers;

	static std::vector<FlameEngine> instances;
	static void init_instances(const int concurrency, const Nonweapon& equip, const Player& player, const FlameMethod method, const FlameCriteria criteria);
	static std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT> make_table(const Nonweapon equip);
	static void set_attack_array(std::array<int, 7>& array, const Weapon& equip);

	void start(const Worker f, const int k);
	void wait();
	void advantage_nonweapon_worker(const int64_t k);
	void nonadvantage_nonweapon_worker(const int64_t k);
	void choose_effect_types(const int effects_count);
	void choose_effect_tiers(const int effects_count);
	int rng();

};

#endif
