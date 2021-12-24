#pragma once
#ifndef GAME_SIGNATURES_H
#define GAME_SIGNATURES_H

#include <Windows.h>
#include <vector>

struct Signature
{
	std::vector<BYTE> pattern;
	size_t index;
};

namespace Signatures
{
	const Signature inputLeft = { { 0xFF, 0x15, '?', '?', '?', '?', 0x88, 0x86, 0x8F, 0x02, 0x00, 0x00, 0x8B, 0x7E, 0x68 }, 1 };
}

#endif // GAME_SIGNATURES_H