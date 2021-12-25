#pragma once
#ifndef GAME_SIGNATURES_H
#define GAME_SIGNATURES_H

#include <Windows.h>
#include <vector>

struct Signature
{
	std::vector<BYTE> pattern;
	BYTE wildcard;
	size_t index;
	ptrdiff_t offset;
};

namespace Signatures
{
	const Signature inputLeft = { { 0x8B, 0xD0, 0x8B, 0xCF, 0xFF, 0x15, '?', '?', '?', '?', 0x88, 0x86, 0x8F, 0x02, 0x00, 0x00, 0x8B, 0x7E, 0x68, 0xB9 }, '?', 1, 4 };
}

#endif // GAME_SIGNATURES_H