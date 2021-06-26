#include "Player.hpp"

Player::Player(const double secondary_ratio, const double att_ratio, const double as_ratio)
	: primary_per_secondary {secondary_ratio}
	, primary_per_att {att_ratio}
	, primary_per_as {as_ratio}
{}

double Player::get_primary_per_secondary() const
{
	return primary_per_secondary;
}

double Player::get_primary_per_att() const
{
	return primary_per_att;
}

double Player::get_primary_per_as() const
{
	return primary_per_as;
}
