#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <Windows.h>
#include <string>
#include <iostream>

#define print(x) (std::cout << x << std::endl)
#define print_error(x) (std::cout << x << " | Error Code: " << GetLastError() << std::endl)

template<typename ... Args>
std::wstring string_format(const std::wstring& format, Args ... args);

#endif // UTIL_H