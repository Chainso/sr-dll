#include "inject/server.h"
#include "inject/thread.h"
#include "inject/ntinfo.h"
#include "inject/detour.h"
#include "game/game.h"
#include "game/memory.h"
#include "game/signatures.h"
#include "game/communication.h"
#include "util.h"

#define INPUT_CALL_SIZE 6
#define LEN_BETWEEN_INPUT_CALLS 34

/**
 * @brief Creates the code detours necessary to run the program.
 * 
 * @return bool True if the detours were created successfully.
 */
static bool CreateDetours()
{
    uintptr_t address = FindSignature(Signatures::inputLeft);
    print("Found address of call before mov 28f (left): " << (LPVOID)address);
    
    // Write the detours in reverse order since the first detour will incur I/O, prevents problem of game reading
    // memory before all detours are written, shouldn't be an issue with this
    address += (NUM_PLAYER_INPUTS - 1) * LEN_BETWEEN_INPUT_CALLS;

    for (int i = NUM_PLAYER_INPUTS - 1; i > 0; i--)
    {
        if (!CallDetour32((BYTE*)address, (uintptr_t)Communication::IsPressed, INPUT_CALL_SIZE))
        {
            print_error("Failed to detour pressed for action: " << i);
            return false;
        }

        address -= LEN_BETWEEN_INPUT_CALLS;
    }

    if (!CallDetour32((BYTE*)address, (uintptr_t)Communication::GetInputAndCheckPressed, INPUT_CALL_SIZE))
    {
        print_error("Failed to detour I/O and check pressed");
        return false;
    }

    return true;
}


/**
 * @brief Starts the game server to receive inputs and export packets.
 * 
 * @return int  The return status of the server, -1 on error.
 */
int GameServer()
{
    print("Getting game thread");
    HANDLE thread = GetFirstThread();
    
    if (thread == INVALID_HANDLE_VALUE) {
        return -1;
    }

    HANDLE process = GetCurrentProcess();

    uintptr_t start_addr = (uintptr_t)GetThreadStartAddress(process, thread);

    CloseHandle(process);
    CloseHandle(thread);

    print("Got thread start address: " << (LPVOID)start_addr);

    Game* game = Game::GetGame(start_addr);
    Communication::Communicator::CreateCommunicator(game, PIPE_NAME);

    if (!CreateDetours())
    {
        // Failed to create detours, exit
        return -1;
    }

    if (!Communication::comm->CreatePipe() || !Communication::comm->ConnectAndSetupPipe())
    {
        // Failed to create pipe or get a connection, error out
        return -1;
    }

    return 0;
}
