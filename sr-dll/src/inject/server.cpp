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

/**
 * @brief Creates the code detours necessary to run the program.
 */
static void CreateDetours()
{
    LPVOID detour_func_addr = (LPVOID)Communication::IsPressed;

    print("Detour func LeftInp: " << detour_func_addr);
    uintptr_t address = FindSignature(Signatures::inputLeft);
    print("Found address of call before mov 28f: " << (LPVOID)address);
    
    if (address != NULL)
    {
        print("Detouring to function LeftInp at: " << detour_func_addr);
        bool detoured = CallDetour32((BYTE*)address, (uintptr_t)detour_func_addr, INPUT_CALL_SIZE);

        print("Detoured: " << detoured);
    }
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

    if (!Communication::comm->CreatePipe())
    {
        // Failed to create pipe, error outop;p
    }

    CreateDetours();

    /*
    Packet packet = CreatePacket(game);

    print("Got sample packet");
    print(packet.length);
    print(packet.message);

    print("Starting server loop");
    int ret_value = GameServerLoop(thread, game);
    CloseHandle(thread);

    return ret_value;
    */


    return 0;
}
