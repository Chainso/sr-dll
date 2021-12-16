#include <windows.h>
#include <tlhelp32.h>

#include "inject/thread.h"

/**
 * @brief Gets the first thread of the current process.
 * 
 * @return HANDLE   The first thread of the current process, or NULL if there is a failure.
 */
HANDLE GetFirstThread()
{
    // Get threadlist
    DWORD processId = GetCurrentProcessId();
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"Failed to get handle", L"Hello there!", MB_ICONINFORMATION);
        return NULL;
    }

    THREADENTRY32 entry;
    entry.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(snapshot, &entry))
    {
        // Failure here
        CloseHandle(snapshot);
        MessageBox(NULL, L"Failed to get thread", L"Hello there!", MB_ICONINFORMATION);
        return NULL;
    }

    // Get the first thread belonging to the process
    while (entry.th32OwnerProcessID != processId)
    {
        Thread32Next(snapshot, &entry);
    }

    CloseHandle(snapshot);

    HANDLE thread = OpenThread(THREAD_QUERY_INFORMATION, false, entry.th32ThreadID);

    return thread;
}