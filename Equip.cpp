#include "Equip.hpp"

Equip::Equip(const int lev, const bool adv)
	: level {lev}
	, advantage {adv}
{}

int Equip::get_level() const
{
	return level;
}

bool Equip::has_advantage() const
{
	return advantage;
}

Nonweapon::Nonweapon(const int lev, const bool adv)
	: Equip(lev, adv)
{}

Weapon::Weapon(const int lev, const int att, const bool adv)
	: Equip(lev, adv)
	, attack {att}
{}
