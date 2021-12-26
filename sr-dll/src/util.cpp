#include <wchar.h>
#include <codecvt>

#include "util.h"

/**
 * @brief Formats a wide string.
 * 
 * @tparam Args         The generic argument type.
 * @param format        The string to format.
 * @param args          The format arguments.
 * @return std::wstring The constructed, formatted wide string. 
 */
template<typename ... Args>
std::wstring string_format(const std::wstring& format, Args ... args)
{
    // Extra space for '\0'
    size_t size = swprintf(nullptr, 0, format.c_str(), args ...) + sizeof(wchar_t);

    if (size <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }

    wchar_t* buf = std::make_unique<wchar_t[]>(size);
    swprintf(buf.get(), size, format.c_str(), args ...);

    // We don't want the '\0' inside
    return std::wstring(buf.get(), buf.get() + size - 1);
}


/**
 * @brief Creates a prefix array to use for KMP searching.
 * 
 * @param pattern   The pattern to create the prefix for.
 * @param wildcard  The wildcard character in the pattern.
 * @return          The prefix array of the pattern.
 */
std::vector<int> CreatePrefix(const std::vector<BYTE>& pattern, BYTE wildcard) {
    std::vector<int> backtrack(pattern.size());
    backtrack[0] = 0;

    int next = backtrack[0];

    for (size_t i = 1; i < backtrack.size(); i++)
    {
        backtrack[i] = next;

        while (next > 0 && pattern[next] != wildcard && pattern[next] != pattern[i])
        {
            next = backtrack[next];
        }

        if (pattern[next] == wildcard || pattern[i] == pattern[next])
        {
            next++;
        }
    }

    return backtrack;
}


/**
 * @brief Finds a byte substring within a byte array, using the already generated prefix array.
 * 
 * @param byte_arr  The array of bytes to search in.
 * @param len       The length of the byte array.
 * @param pattern   The byte pattern to search for.
 * @param wildcard  The wildcard character in the pattern.
 * @param prefix    The prefix array of the pattern.
 * @return          The offset to the start of the matched pattern, or -1 on failure.
 */
ptrdiff_t search(BYTE* byte_arr, size_t len, const std::vector<BYTE>& pattern, BYTE wildcard, const std::vector<int>& prefix) {
    size_t i;
    size_t current = 0;

    for (i = 0; i < len && current < pattern.size(); i++)
    {
        if (&byte_arr[i] == pattern.data())
        {
            // Make sure not the match the pattern on its own memory
            i += pattern.size() - 1;
            current = 0;
            continue;
        }

        while (current > 0 && pattern[current] != wildcard && byte_arr[i] != pattern[current])
        {
            current = prefix[current];
        }

        if (pattern[current] == wildcard || byte_arr[i] == pattern[current])
        {
            current++;
        }
    }

    if (current == pattern.size())
    {
        return i - pattern.size();
    }

    return -1;
}

/**
 * @brief Finds a byte substring within a byte array.
 *
 * @param byte_arr  The array of bytes to search in.
 * @param len       The length of the byte array.
 * @param pattern   The byte pattern to search for.
 * @param wildcard  The wildcard character in the pattern.
 * @return          The offset to the start of the matched pattern, or -1 on failure.
 */
ptrdiff_t search(BYTE* byte_arr, size_t len, const std::vector<BYTE>& pattern, BYTE wildcard) {
    std::vector<int> prefix = CreatePrefix(pattern, wildcard);
    return search(byte_arr, len, pattern, wildcard, prefix);
}