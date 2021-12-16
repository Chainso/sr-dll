#pragma once
#ifndef MEMORY_H
#define MEMORY_H	

#include <cstddef>
#include <vector>

uintptr_t FindAddress(uintptr_t pointer, std::vector<ptrdiff_t> offsets);

#endif // MEMORY_H