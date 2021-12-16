#include <cstddef>

#include "game/memory.h"

/**
 * @brief Finds the address using the pointer chain with offsets given.
 * 
 * @param pointer       The most indirect pointer for the address.
 * @param offsets       The offsets at each pointer level.
 * @return uintptr_t    The resulting pointer from computing the addresses.
 */
uintptr_t FindAddress(uintptr_t pointer, std::vector<ptrdiff_t> offsets)
{
    for (int i = 0; i < offsets.size(); i++) {
        pointer = *(uintptr_t*)pointer + offsets[i];
    }

    return pointer;
}
