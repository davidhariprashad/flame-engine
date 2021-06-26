#include "Flame.hpp"

std::ostream& operator<<(std::ostream& os, const Flame& f)
{
	std::for_each(f.effects.cbegin(), f.effects.cend(), [&os](int e) mutable
	{
		if (e)
		{
			os << e << '\n';
		}
	});
	return os;
}

void Flame::add(const FlameType type, const int value)
{
	(*operations[to_underlying(type)])(*this, value);
}

double Flame::score(const FlameCriteria c, const Player& p) const
{
	return (this->*score_functions[to_underlying(c)])(p);
}

double Flame::score(const double primary, const double secondary, const double att, const double as, const Player& p)
{
	return primary
		+ p.get_primary_per_secondary()*secondary
		+ p.get_primary_per_att()*att
		+ p.get_primary_per_as()*as;
}

int Flame::get_stat(const FlameStat s) const
{
	return effects[to_underlying(s)];
}

double Flame::str_score(const Player& p) const
{
	return score(get_stat(FlameStat::STR), get_stat(FlameStat::DEX), get_stat(FlameStat::ATT), get_stat(FlameStat::AS), p);
}

double Flame::dex_score(const Player& p) const
{
	return score(get_stat(FlameStat::DEX), get_stat(FlameStat::STR), get_stat(FlameStat::ATT), get_stat(FlameStat::AS), p);
}

double Flame::int_score(const Player& p) const
{
	return score(get_stat(FlameStat::INT), get_stat(FlameStat::LUK), get_stat(FlameStat::MATT), get_stat(FlameStat::AS), p);
}

double Flame::luk_score(const Player& p) const
{
	return score(get_stat(FlameStat::LUK), get_stat(FlameStat::DEX), get_stat(FlameStat::ATT), get_stat(FlameStat::AS), p);
}

double Flame::fd_score(const Player& p) const
{
	return 0.0;
}

void Flame::clear()
{
	std::fill(effects.begin(), effects.end(), 0);
}

void Flame::print_nonweapon_effects() const
{
	std::for_each(effects.begin(), effects.end(), [index = 0](int e) mutable
	{
		if (e)
		{
			std::cout << Flame::nonweapon_labels[index] << ' ' << e << '\n';
		}
		++index;
	});
}

void Flame::print_weapon_effects() const
{
	std::for_each(effects.begin(), effects.end(), [index = 0](int e) mutable
	{
		if (e)
		{
			std::cout << Flame::weapon_labels[index] << ' ' << e << '\n';
		}
		++index;
	});
}

void Flame::add_stat(Flame& flame, const FlameStat stat, const int value)
{
	flame.effects[to_underlying(stat)] += value;
}

void Flame::add_str(Flame& flame, const int value)
{
	add_stat(flame, FlameStat::STR, value);
}

void Flame::add_dex(Flame& flame, const int value)
{
	add_stat(flame, FlameStat::DEX, value);
}

void Flame::add_int(Flame& flame, const int value)
{
	add_stat(flame, FlameStat::INT, value);
}

void Flame::add_luk(Flame& flame, const int value)
{
	add_stat(flame, FlameStat::LUK, value);
}

void Flame::add_str_dex(Flame& flame, const int value)
{
	add_str(flame, value);
	add_dex(flame, value);
}

void Flame::add_str_int(Flame& flame, const int value)
{
	add_str(flame, value);
	add_int(flame, value);
}

void Flame::add_str_luk(Flame& flame, const int value)
{
	add_str(flame, value);
	add_luk(flame, value);
}

void Flame::add_dex_int(Flame& flame, const int value)
{
	add_dex(flame, value);
	add_int(flame, value);
}

void Flame::add_dex_luk(Flame& flame, const int value)
{
	add_dex(flame, value);
	add_luk(flame, value);
}

void Flame::add_int_luk(Flame& flame, const int value)
{
	add_int(flame, value);
	add_luk(flame, value);
}

void Flame::insert_hp(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::HP)] = value;
}

void Flame::insert_mp(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::MP)] = value;
}

void Flame::insert_lev(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::LEV)] = value;
}

void Flame::insert_def(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::DEF)] = value;
}

void Flame::insert_att(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::ATT)] = value;
}

void Flame::insert_matt(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::MATT)] = value;
}

void Flame::insert_speed_bd(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::SPEED_BD)] = value;
}

void Flame::insert_jump_dmg(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::JUMP_DMG)] = value;
}

void Flame::insert_as(Flame& flame, const int value)
{
	flame.effects[to_underlying(FlameStat::AS)] = value;
}
