#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <string>

template<typename ... Args>
std::wstring string_format(const std::wstring& format, Args ... args);

#endif // UTIL_H