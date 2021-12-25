#pragma once
#ifndef MEMORY_H
#define MEMORY_H	

#include <cstddef>
#include <vector>
#include <Windows.h>

#include "game/signatures.h"

uintptr_t FindAddress(uintptr_t pointer, std::vector<ptrdiff_t> offsets);
uintptr_t FindSignature(BYTE* byte_arr, size_t len, std::vector<BYTE> pattern, BYTE wildcard);
uintptr_t FindSignature(std::vector<BYTE> pattern, BYTE wildcard);
uintptr_t FindSignature(BYTE* byte_arr, size_t len, Signature sig);
uintptr_t FindSignature(Signature sig);

#endif // MEMORY_H