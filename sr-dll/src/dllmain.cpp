// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

#include "util.h"
#include "inject/server.h" 


/**
 * @brief A thread to run the DLL server.
 * 
 * @return DWORD    The return status of the thread.
 */
DWORD WINAPI ServerThread(HMODULE hModule)
{
    // Create a debug console
    FILE* std_file;
    if (AllocConsole() == 0
        || freopen_s(&std_file, "CONIN$", "r", stdin) != 0
        || freopen_s(&std_file, "CONOUT$", "w", stderr) != 0
        || freopen_s(&std_file, "CONOUT$", "w", stdout) != 0)
    {
        MessageBox(NULL, L"Error creating a console", L"SpeedRunners DLL Error", MB_ICONERROR);
        return -1;
    }

    print("Starting game server");
    DWORD return_value = GameServer();

    print("Closing game server");
    // Perhaps keep console so hooks can use prints
    //FreeConsole();

    // Comment out for now until server is fully working
    //FreeLibraryAndExitThread(hModule, return_value);

    return return_value;
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
                MessageBox(NULL, L"Error creating server thread", L"SpeedRunners DLL Error", MB_ICONERROR);
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
