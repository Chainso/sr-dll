#pragma once
#ifndef GAME_OFFSETS_H
#define GAME_OFFSETS_H

#include <cstddef>
#include <vector>

struct GameAddress
{
	ptrdiff_t base;
	std::vector<ptrdiff_t> offsets;
};

namespace AddressOffsets
{
	const GameAddress player = { -0x980, {0x188, 0xEE4, 0x18, 0x44, 0x10, 0x48, 0x0, 0x10} };
	const GameAddress game = { -0x994, {0x65C, 0x260, 0x10, 0xE0, 0x3C, 0x80, 0x0} };
}

#endif // GAME_OFFSETS_H