#include "game/memory.h"
#include "util.h"

/**
 * @brief Finds the address using the pointer chain with offsets given.
 * 
 * @param pointer       The most indirect pointer for the address.
 * @param offsets       The offsets at each pointer level.
 * @return uintptr_t    The resulting pointer from computing the addresses.
 */
uintptr_t FindAddress(uintptr_t pointer, const std::vector<ptrdiff_t>& offsets)
{
    for (size_t i = 0; i < offsets.size(); i++) {
        pointer = *(uintptr_t*)pointer + offsets[i];
    }

    return pointer;
}


/**
 * @brief Finds the first subarray that matches a signature.
 *
 * @param byte_arr  The array of bytes to search in.
 * @param len       The length of the byte array.
 * @param pattern   The byte pattern to search for.
 * @param wildcard  The wildcard character in the pattern.
 * @return          A pointer to the start of the matched pattern or NULL.
 */
uintptr_t FindSignature(BYTE* byte_arr, size_t len, const std::vector<BYTE>& pattern, BYTE wildcard)
{
    BYTE* end = byte_arr + len;
    print("Starting addr: " << (LPVOID)byte_arr << " | Ending addr: " << (LPVOID)end);

    MEMORY_BASIC_INFORMATION mbi{ 0 };
    DWORD protection_flags = (PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS);

    std::vector<int> prefix = CreatePrefix(pattern, wildcard);

    for (BYTE* current = byte_arr; current < end; current += mbi.RegionSize)
    {
        // Ignore regions without committed memory and protected regions
        if (VirtualQuery((LPCVOID)current, &mbi, sizeof(mbi))
            && !(mbi.Protect & protection_flags || !(mbi.State & MEM_COMMIT)))
        {
            print("Searching region: " << (LPVOID)current);
            ptrdiff_t offset = search(current, mbi.RegionSize, pattern, wildcard, prefix);

            if (offset != -1)
            {
                return (uintptr_t)(current + offset);
            }
        }
    }

    return NULL;
}


/**
 * @brief Finds the first address that matches a signature in the current module.
 *
 * @param pattern   The byte pattern to search for.
 * @param wildcard  The wildcard character in the pattern.
 * @return          A pointer to the start of the matched pattern or NULL.
 */
uintptr_t FindSignature(const std::vector<BYTE>& pattern, BYTE wildcard)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    BYTE* start = (BYTE*)si.lpMinimumApplicationAddress;
    BYTE* end = (BYTE*)si.lpMaximumApplicationAddress;
    size_t len = end - start;

    return FindSignature(start, len, pattern, wildcard);
}


/**
 * @brief Finds the first address that matches a signature in the current module.
 *
 * @param byte_arr  The array of bytes to search in.
 * @param len       The length of the byte array.
 * @param sig       The signature to match.
 * @return          A pointer to the start of the matched pattern or NULL.
 */
uintptr_t FindSignature(BYTE* byte_arr, size_t len, const Signature& sig)
{
    BYTE* end = byte_arr + len;
    BYTE* current = byte_arr;

    for (size_t i = 0; i <= sig.index; i++)
    {
        len = end - current;
        current = (BYTE*)FindSignature(current, len, sig.pattern, sig.wildcard);

        if (current == NULL)
        {
            return NULL;
        }

        current++;
    }

    current--;

    return (uintptr_t)current + sig.offset;
}


/**
 * @brief Finds the first address that matches a signature in the current module.
 *
 * @param sig   The signature to match.
 * @return      A pointer to the start of the matched pattern or NULL.
 */
uintptr_t FindSignature(const Signature& sig)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    BYTE* start = (BYTE*)si.lpMinimumApplicationAddress;
    BYTE* end = (BYTE*)si.lpMaximumApplicationAddress;
    size_t len = end - start;

    return FindSignature(start, len, sig);
}