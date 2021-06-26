#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player
{
public:
	Player(const double secondary_ratio, const double att_ratio, const double as_ratio);
	double get_primary_per_secondary() const;
	double get_primary_per_att() const;
	double get_primary_per_as() const;
private:
	double primary_per_secondary;
	double primary_per_att;
	double primary_per_as;
};

#endif
