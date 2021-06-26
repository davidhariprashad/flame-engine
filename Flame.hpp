#ifndef FLAME_HPP
#define FLAME_HPP

#include "FlameTraits.hpp"
#include "Player.hpp"

#include <iostream>
#include <array>
#include <map>
#include <algorithm>

class Flame
{

	typedef void (*op_ptr)(Flame&, const int);
	typedef double (Flame::*score_function)(const Player&) const;
	friend std::ostream& operator<<(std::ostream& os, const Flame& f);

public:

	void add(const FlameType type, const int value);
	void clear();
	double score(const FlameCriteria m, const Player& p) const;
	void print_nonweapon_effects() const;
	void print_weapon_effects() const;

	constexpr static std::array<std::string_view, FlameTraits::STAT_COUNT> nonweapon_labels
	{
		"STR:",
		"DEX:",
		"INT:",
		"LUK:",
		"HP:",
		"MP:",
		"LEV:",
		"DEF:",
		"ATT:",
		"MATT:",
		"SPEED:",
		"JUMP:",
		"All Stats %:"
	};

	constexpr static std::array<std::string_view, FlameTraits::STAT_COUNT> weapon_labels
	{
		"STR:",
		"DEX:",
		"INT:",
		"LUK:",
		"HP:",
		"MP:",
		"LEV:",
		"DEF:",
		"ATT:",
		"MATT:",
		"Damage %:",
		"Boss Damage %:",
		"All Stats %:"
	};

private:

	static void add_stat(Flame& flame, const FlameStat type, const int value);
	static void add_str(Flame& flame, const int value);
	static void add_dex(Flame& flame, const int value);
	static void add_int(Flame& flame, const int value);
	static void add_luk(Flame& flame, const int value);
	static void add_str_dex(Flame& flame, const int value);
	static void add_str_int(Flame& flame, const int value);
	static void add_str_luk(Flame& flame, const int value);
	static void add_dex_int(Flame& flame, const int value);
	static void add_dex_luk(Flame& flame, const int value);
	static void add_int_luk(Flame& flame, const int value);
	static void insert_hp(Flame& flame, const int value);
	static void insert_mp(Flame& flame, const int value);
	static void insert_lev(Flame& flame, const int value);
	static void insert_def(Flame& flame, const int value);
	static void insert_att(Flame& flame, const int value);
	static void insert_matt(Flame& flame, const int value);
	static void insert_speed_bd(Flame& flame, const int value);
	static void insert_jump_dmg(Flame& flame, const int value);
	static void insert_as(Flame& flame, const int value);
	static double score(const double primary, const double secondary, const double att, const double as, const Player& p);
	int get_stat(const FlameStat s) const;
	double get_metric(const FlameCriteria m) const;
	double str_score(const Player& p) const;
	double dex_score(const Player& p) const;
	double int_score(const Player& p) const;
	double luk_score(const Player& p) const;
	double fd_score(const Player& p) const;

	constexpr static std::array<op_ptr, FlameTraits::TYPE_COUNT> operations
	{
		&Flame::add_str,
		&Flame::add_dex,
		&Flame::add_int,
		&Flame::add_luk,
		&Flame::add_str_dex,
		&Flame::add_str_int,
		&Flame::add_str_luk,
		&Flame::add_dex_int,
		&Flame::add_dex_luk,
		&Flame::add_int_luk,
		&Flame::insert_hp,
		&Flame::insert_mp,
		&Flame::insert_lev,
		&Flame::insert_def,
		&Flame::insert_att,
		&Flame::insert_matt,
		&Flame::insert_speed_bd,
		&Flame::insert_jump_dmg,
		&Flame::insert_as
	};

	constexpr static std::array<score_function, FlameTraits::CRITERIA_COUNT> score_functions
	{
		&Flame::str_score,
		&Flame::dex_score,
		&Flame::int_score,
		&Flame::luk_score,
		&Flame::fd_score
	};

	std::array<int, FlameTraits::STAT_COUNT> effects {};

};

#endif
