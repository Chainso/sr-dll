#pragma once
#ifndef GAME_OFFSETS_H
#define GAME_OFFSETS_H

#include <vector>

struct GameAddress
{
	ptrdiff_t base;
	std::vector<ptrdiff_t> offsets;
};

namespace AddressOffsets
{
	extern const GameAddress player;
	extern const GameAddress game;
}

#endif // GAME_OFFSETS_H