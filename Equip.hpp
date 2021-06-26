#ifndef EQUIP_HPP
#define EQUIP_HPP

class Equip
{
public:
	int get_level() const;
	bool has_advantage() const;
protected:
	Equip() = default;
	Equip(const int lev, const bool adv);
private:
	int level;
	bool advantage;
};

class Nonweapon : public Equip
{
 public:
	Nonweapon() = default;
	Nonweapon(const int lev, const bool adv = true);
};

class Weapon : public Equip
{
public:
	Weapon() = default;
	Weapon(const int lev, const int att, const bool adv = true);
private:
	int attack;
};

#endif
