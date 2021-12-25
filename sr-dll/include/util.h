#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

#define print(x) (std::cout << x << std::endl)
#define print_error(x) (std::cout << x << " | Error Code: " << GetLastError() << std::endl)

template<typename ... Args>
std::wstring string_format(const std::wstring& format, Args ... args);
std::vector<int> CreatePrefix(std::vector<BYTE> pattern, BYTE wildcard);
ptrdiff_t search(BYTE* byte_arr, size_t len, std::vector<BYTE> pattern, BYTE wildcard, std::vector<int> prefix);
ptrdiff_t search(BYTE* byte_arr, size_t len, std::vector<BYTE> pattern, BYTE wildcard);

#endif // UTIL_H