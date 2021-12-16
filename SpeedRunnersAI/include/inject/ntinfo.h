// Credits to https://github.com/makemek/cheatengine-threadstack-finder
#pragma once
#ifndef NTINFO_H
#define NTINFO_H

DWORD GetThreadStartAddress(HANDLE processHandle, HANDLE hThread);
void* GetThreadStackTopAddress_x86(HANDLE hProcess, HANDLE hThread);

#endif