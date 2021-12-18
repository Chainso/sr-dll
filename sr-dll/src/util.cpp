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
