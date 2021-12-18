#include <windows.h>
#include <tlhelp32.h>

#include "util.h"
#include "inject/thread.h"

/**
 * @brief Gets the first thread of the current process.
 * 
 * @return HANDLE   The first thread of the current process, or INVALID_HANDLE_VALUE if there is a failure.
 */
HANDLE GetFirstThread()
{
    // Get threadlist
    DWORD processId = GetCurrentProcessId();
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        print_error("Failed to get thread snapshot");
        return INVALID_HANDLE_VALUE;
    }

    THREADENTRY32 entry;
    entry.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(snapshot, &entry))
    {
        // Failure here
        CloseHandle(snapshot);
        print_error("Failed to get first thread of snapshot");
        return INVALID_HANDLE_VALUE;
    }

    // Get the first thread belonging to the process
    while (entry.th32OwnerProcessID != processId)
    {
        Thread32Next(snapshot, &entry);
    }

    CloseHandle(snapshot);

    HANDLE thread = OpenThread(THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME, false, entry.th32ThreadID);

    if (thread == INVALID_HANDLE_VALUE)
    {
        print_error("Failed to open first game thread");
    }

    return thread;
}