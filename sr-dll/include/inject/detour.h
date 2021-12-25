#pragma once
#ifndef INJECT_DETOUR_H
#define INJECT_DETOUR_H

#include <Windows.h>

bool CallDetour32(BYTE* dest, uintptr_t function, size_t len);

#endif // INJECT_DETOUR_H