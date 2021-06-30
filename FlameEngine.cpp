#include "FlameEngine.hpp"

std::vector<FlameEngine> FlameEngine::instances;

FlameEngine::FlameEngine(const Player& p, const FlameMethod m, const FlameCriteria c, const std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT>& table, const std::vector<int>& t_pool, const std::vector<int>& c_pool)
	: distribution {std::uniform_int_distribution<int>(0, 99)}
	, player {p}
	, method {m}
	, criteria {c}
	, stat_table {table}
	, tier_pool {t_pool}
	, effect_count_pool {c_pool}
{
	engine.seed(std::random_device{}());
	std::for_each(effect_type_pool.begin(), effect_type_pool.end(), [i = 0](FlameType& t) mutable
	{
		t = static_cast<FlameType>(i++);
	});
	types.reserve(4);
	tiers.reserve(4);
}

FlameEngine::ScoreHistogram FlameEngine::make_histogram(const Nonweapon e, const Player p, const FlameMethod m, const FlameCriteria criteria, const int64_t n, const int c)
{
	init_instances(c, e, p, m, criteria);
	ScoreHistogram global_histogram{};
	std::for_each(instances.begin(), instances.end(), [adv = e.has_advantage(), k = n/c](FlameEngine& instance)
	{
		instance.start(adv ? &FlameEngine::advantage_nonweapon_worker : &FlameEngine::nonadvantage_nonweapon_worker, k);
	});
	std::for_each(instances.begin(), instances.end(), [&global_histogram](FlameEngine& instance)
	{
		instance.wait();
		std::transform(instance.score_histogram.cbegin(), instance.score_histogram.cend(), global_histogram.cbegin(), global_histogram.begin(), std::plus<int>());
	});
	return global_histogram;
}

void FlameEngine::start(const FlameEngine::Worker f, const int k)
{
	thread = std::thread(f, this, k);
}

void FlameEngine::wait()
{
	thread.join();
}

void FlameEngine::init_instances(const int concurrency, const Nonweapon& equip, const Player& player, const FlameMethod method, const FlameCriteria criteria)
{
	const std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT> table {make_table(equip)};
	const std::array<int, 5>& tier_percents_ref = lookup_array_ref<int, 5>(tier_percents_table.cbegin(), tier_percents_table.cend(), method, std::equal_to<FlameMethod>());
	std::vector<int> t_pool(100);
	make_pool(tier_percents_ref.cbegin(), tier_percents_ref.cend(), t_pool.begin(), equip.has_advantage() ? 1+2 : 1);
	std::vector<int> c_pool(100);
	make_pool(nonadvantage_effect_counts.cbegin(), nonadvantage_effect_counts.cend(), c_pool.begin(), 1);
	instances.clear();
	instances.reserve(concurrency);
	for (int i {}; i < concurrency; ++i)
	{
		instances.push_back(FlameEngine(player, method, criteria, table, t_pool, c_pool));
	}
}

void FlameEngine::advantage_nonweapon_worker(const int64_t k)
{
	std::fill(score_histogram.begin(), score_histogram.end(), 0);
	const int effect_count {advantage_effects_count()};
	for (int64_t i {}; i < k; ++i)
	{
		Flame f;
		choose_effect_types(effect_count);
		choose_effect_tiers(effect_count);
		auto type_it = types.cbegin();
		auto tier_it = tiers.cbegin();
		for (; type_it != types.cend() && tier_it != tiers.cend(); ++type_it, ++tier_it)
		{
			f.add(static_cast<FlameType>(*type_it), stat_table[to_underlying(*type_it)][*tier_it - 1]);
		}
		const int score {static_cast<int>(std::round(f.score(criteria, player)))};
		++score_histogram[score];
	}
}

void FlameEngine::nonadvantage_nonweapon_worker(const int64_t k)
{
	std::fill(score_histogram.begin(), score_histogram.end(), 0);
	for (int64_t i {}; i < k; ++i)
	{
		Flame f;
		const int effect_count {nonadvantage_effects_count()};
		choose_effect_types(effect_count);
		choose_effect_tiers(effect_count);
		auto type_it = types.cbegin();
		auto tier_it = tiers.cbegin();
		for (; type_it != types.cend() && tier_it != tiers.cend(); ++type_it, ++tier_it)
		{
			f.add(static_cast<FlameType>(*type_it), stat_table[to_underlying(*type_it)][*tier_it - 1]);
		}
		++score_histogram[static_cast<int>(std::round(f.score(criteria, player)))];
	}
}

std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT> FlameEngine::make_table(const Nonweapon equip)
{
	std::array<std::array<int, 7>, FlameTraits::TYPE_COUNT> table;
	table[to_underlying(FlameType::STR)] = lookup_array_ref<int, 7>(str_dex_int_luk_def_table.cbegin(), str_dex_int_luk_def_table.cend(), equip.get_level(), std::less_equal<int>());
	table[to_underlying(FlameType::DEX)] = table[to_underlying(FlameType::STR)];
	table[to_underlying(FlameType::INT)] = table[to_underlying(FlameType::STR)];
	table[to_underlying(FlameType::LUK)] = table[to_underlying(FlameType::STR)];
	table[to_underlying(FlameType::STR_DEX)] = lookup_array_ref<int, 7>(pair_stat_table.cbegin(), pair_stat_table.cend(), equip.get_level(), std::less_equal<int>());
	table[to_underlying(FlameType::STR_INT)] = table[to_underlying(FlameType::STR_DEX)];
	table[to_underlying(FlameType::STR_LUK)] = table[to_underlying(FlameType::STR_DEX)];
	table[to_underlying(FlameType::DEX_INT)] = table[to_underlying(FlameType::STR_DEX)];
	table[to_underlying(FlameType::DEX_LUK)] = table[to_underlying(FlameType::STR_DEX)];
	table[to_underlying(FlameType::INT_LUK)] = table[to_underlying(FlameType::STR_DEX)];
	table[to_underlying(FlameType::HP)] = lookup_array_ref<int, 7>(hp_mp_table.cbegin(), hp_mp_table.cend(), equip.get_level(), std::less_equal<int>());
	table[to_underlying(FlameType::MP)] = table[to_underlying(FlameType::HP)];
	table[to_underlying(FlameType::LEV)] = lookup_array_ref<int, 7>(lev_table.cbegin(), lev_table.cend(), equip.get_level(), std::less_equal<int>());
	table[to_underlying(FlameType::DEF)] = table[to_underlying(FlameType::STR)];
	table[to_underlying(FlameType::ATT)] = lookup_array_ref<int, 7>(atk_matk_speed_jump_dmg_as_table.cbegin(), atk_matk_speed_jump_dmg_as_table.cend(), equip.get_level(), std::less_equal<int>());
	table[to_underlying(FlameType::MATT)] = table[to_underlying(FlameType::ATT)];
	table[to_underlying(FlameType::SPEED_BD)] = table[to_underlying(FlameType::ATT)];
	table[to_underlying(FlameType::JUMP_DMG)] = table[to_underlying(FlameType::ATT)];
	table[to_underlying(FlameType::AS)] = table[to_underlying(FlameType::ATT)];
	return table;
}

int FlameEngine::advantage_effects_count() const
{
	return 4;
}

int FlameEngine::nonadvantage_effects_count()
{
	return tier_pool[rng()];
}

void FlameEngine::choose_effect_types(const int effects_count)
{
	types.clear();
	std::sample(effect_type_pool.begin(), effect_type_pool.end(), std::back_inserter(types), effects_count, engine);
}

void FlameEngine::choose_effect_tiers(const int effects_count)
{
	tiers.resize(effects_count);
	std::for_each(tiers.begin(), tiers.end(), [this](int& tier)
	{
		tier = tier_pool[rng()];
	});
}

int FlameEngine::rng()
{
	return distribution(engine);
}
