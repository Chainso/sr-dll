// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <tlhelp32.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <winternl.h>
#include <libloaderapi.h>
#include <Psapi.h>

#include "inject/ntinfo.h"
#include "inject/server.h"
#include "game/memory.h"
#include "util.h"

/**
 * @brief A thread to run the DLL server.
 * 
 * @return DWORD    The return status of the thread.
 */
DWORD WINAPI ServerThread()
{
    return GameServer();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ServerThread, hModule, 0, nullptr);

            if (thread == NULL) {
                MessageBox(NULL, L"Error creating server thread", L"SpeedRunnersAI DLL Error", MB_ICONINFORMATION);
            }
            else {
                CloseHandle(thread);
            }

            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
