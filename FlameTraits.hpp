#ifndef FLAMETRAITS_HPP
#define FLAMETRAITS_HPP

#include <type_traits>

namespace FlameTraits
{

constexpr int TYPE_COUNT {19};
constexpr int STAT_COUNT {13};
constexpr int METHOD_COUNT {9};
constexpr int CRITERIA_COUNT {5};

} // namespace FlameTraits

template <typename E>
constexpr std::underlying_type_t<E> to_underlying(const E e)
{
	return static_cast<std::underlying_type_t<E>>(e);
}

enum class FlameMethod : int
{
	DROP_NPC_SHARD,
	SHARD_CHANCE_TIME,
	POWERFUL,
	ETERNAL,
	CRAFTING_FUSING,
	MASTER_CRAFT,
	MASTER_FUSE,
	MEISTER_CRAFT,
	MEISTER_FUSE
};

enum class FlameType : int
{
	STR,
	DEX,
	INT,
	LUK,
	STR_DEX,
	STR_INT,
	STR_LUK,
	DEX_INT,
	DEX_LUK,
	INT_LUK,
	HP,
	MP,
	LEV,
	DEF,
	ATT,
	MATT,
	SPEED_BD,
	JUMP_DMG,
	AS
};

enum class FlameStat : int
{
	STR,
	DEX,
	INT,
	LUK,
	HP,
	MP,
	LEV,
	DEF,
	ATT,
	MATT,
	SPEED_BD,
	JUMP_DMG,
	AS
};

enum class FlameCriteria : int
{
	STR,
	DEX,
	INT,
	LUK,
	FD
};

#endif
