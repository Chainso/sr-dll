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
	extern const Signature inputLeft;
}

#endif // GAME_SIGNATURES_H