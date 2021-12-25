#include "inject/detour.h"
#include "util.h"

#define CALL_INSTRUCTION 0xE8
#define CALL_INSTRUCTION_SIZE 5
#define NOOP 0x90

bool CallDetour32(BYTE* dest, uintptr_t function, size_t len)
{
	if (len < CALL_INSTRUCTION_SIZE)
	{
		return false;
	}

	DWORD old_protection;
	if (!VirtualProtect(dest, CALL_INSTRUCTION_SIZE, PAGE_EXECUTE_READWRITE, &old_protection))
	{
		return false;
	}

	uintptr_t detour_func_offset = function - (uintptr_t)dest - CALL_INSTRUCTION_SIZE;
	print("Call detour: " << (LPVOID)dest << " " << (LPVOID)function << " " << (LPVOID)detour_func_offset);

	*dest = CALL_INSTRUCTION;
	*(uintptr_t*)(dest + 1) = detour_func_offset;
	memset(dest + CALL_INSTRUCTION_SIZE, NOOP, len - CALL_INSTRUCTION_SIZE);

	VirtualProtect(dest, CALL_INSTRUCTION_SIZE, old_protection, &old_protection);

	return true;
}
