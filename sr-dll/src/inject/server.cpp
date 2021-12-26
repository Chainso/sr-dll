#include <atlstr.h>

#include "inject/server.h"
#include "inject/thread.h"
#include "inject/ntinfo.h"
#include "inject/detour.h"
#include "game/game.h"
#include "game/packet.h"
#include "game/memory.h"
#include "game/signatures.h"
#include <unordered_map>

#include "game/structs.h"
#include "util.h"

#define INPUT_CALL_SIZE 6

struct IsPressedArg {
    char pad_0[8];
    wchar_t action[0]; // UTF-16 null terminated string
};

enum class PressedAction {
    PS_LEFT,
    PS_RIGHT,
    PS_JUMP,
    PS_WEAPON,
    PS_ITEM,
    PS_TAUNT,
    PS_SWAP,
    PS_SLIDE,
    PS_BOOST
};


// Global vars to use in detours
static const std::unordered_map<std::wstring, PressedAction> pressedActionTable = {
    {L"ps_left", PressedAction::PS_LEFT},
    {L"ps_right", PressedAction::PS_RIGHT},
    {L"ps_jump", PressedAction::PS_JUMP},
    {L"ps_weapon", PressedAction::PS_WEAPON},
    {L"ps_item", PressedAction::PS_ITEM},
    {L"ps_taunt", PressedAction::PS_TAUNT},
    {L"ps_swap", PressedAction::PS_SWAP},
    {L"ps_slide", PressedAction::PS_SLIDE},
    {L"ps_boost", PressedAction::PS_BOOST}
};

static HANDLE pipe;
static bool connected;
static PlayerInput controller;

static int __fastcall IsPressed(void* arg, IsPressedArg* pressed_arg) {
    std::wstring act_str(pressed_arg->action);
    auto act_it = pressedActionTable.find(act_str);

    if (act_it == pressedActionTable.end())
    {
        // Invalid action
        std::wcout << "Invalid action: " << act_str << std::endl;
        return 0;
    }

    PressedAction action = act_it->second;

    return 1;
}

static void CreateDetours()
{
    print("Detour func LeftInp: " << (LPVOID)IsPressed);
    uintptr_t address = FindSignature(Signatures::inputLeft);
    print("Found address of call before mov 28f: " << (LPVOID)address);
    
    if (address != NULL)
    {
        print("Detouring to function LeftInp at: " << (LPVOID)IsPressed);
        bool detoured = CallDetour32((BYTE*)address, (uintptr_t)IsPressed, INPUT_CALL_SIZE);

        print("Detoured: " << detoured);
    }
}

/**
 * @brief Starts a connection to the pipe and sets up the pipe handle state.
 *
 * @param pipe  The pipe that handles connections.
 * @return      0 on success, -1 otherwise.
 */
static int ConnectAndSetupPipe(HANDLE pipe)
{
    // Now set pipe state to wait so we can wait on connections
    DWORD pipe_mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    if (SetNamedPipeHandleState(pipe, &pipe_mode, NULL, NULL) == 0)
    {
        print_error("Failed to set pipe state to wait");

        if (DisconnectNamedPipe(pipe) == 0)
        {
            // Failed to disconnect
            print_error("Failed to disconnect pipe");
        }

        return -1;
    }

    // Wait for client to connect
    print("Waiting for client...");
    if (ConnectNamedPipe(pipe, NULL) == 0)
    {
        // Failed to connect the client
        print_error("Failed to connect to client");
        return -1;
    }

    print("Connected to client!");

    // Now set pipe state to no wait
    pipe_mode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
    if (SetNamedPipeHandleState(pipe, &pipe_mode, NULL, NULL) == 0)
    {
        print_error("Failed to set pipe state to no wait");

        if (DisconnectNamedPipe(pipe) == 0)
        {
            // Failed to disconnect
            print_error("Failed to disconnect pipe");
        }

        return -1;
    }

    return 0;
}

/**
 * @brief The server loop that allows for reconnections once a connection is closed.
 *
 * @param thread    The thread the game is running on.
 * @param game  The current running game.
 * @param pipe  The pipe that handles connections.
 * @return      0 on success, -1 otherwise.
 */
static int GameServerReconnectLoop(HANDLE thread, Game* game, HANDLE pipe)
{
    char read_buffer[READ_BUFFER_SIZE];
    DWORD bytes_read;
    DWORD bytes_written;

    // Connect to client
    while (ConnectAndSetupPipe(pipe) == 0)
    {
        while (true)
        {
            // Read cycle
            if (ReadFile(pipe, read_buffer, READ_BUFFER_SIZE, &bytes_read, NULL))
            {
                // Read succeeded, TODO: do something here

            }
            else if (GetLastError() == ERROR_HANDLE_EOF)
            {
                // EOF, exit out
                print("Read EOF, disconnecting");
                break;
            }
            else if (GetLastError() != ERROR_NO_DATA)
            {
                // Read failed, and not because of no data
                print_error("Failed to read from pipe, disconnecting");
                break;
            }

            // Write cycle
            Packet packet = CreatePacket(game);

            if (!WriteFile(pipe, packet.message.c_str(), packet.length, &bytes_written, NULL))
            {
                print_error("Failed to write to pipe, disconnecting");
                break;
            }

            if (bytes_written > 0)
            {
                print("Sent packet");
                print("Length: " << packet.length);
                print(packet.message);
            }
        }

        // Disconnect from client
        if (DisconnectNamedPipe(pipe) == 0)
        {
            // Failed to disconnect
            print_error("Failed to disconnect pipe");
        }
    }

    return -1;
}

/**
 * @brief The main game server loop.
 * 
 * @param thread    The thread the game is running on.
 * @param game      The current running game.
 * @return          0 on success, -1 otherwise.
 */
static int GameServerLoop(HANDLE thread, Game* game)
{
    // Create named pipe to use
    LPCWSTR pipe_name = L"\\\\.\\pipe\\SpeedRunners-dll";
    
    pipe = CreateNamedPipe(
        pipe_name, 
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
        1,
        WRITE_BUFFER_SIZE,
        READ_BUFFER_SIZE,
        0,
        NULL
    );

    if (pipe == INVALID_HANDLE_VALUE)
    {
        print_error("Failed to create named pipe: " << CW2A(pipe_name));
        return -1;
    }

    return GameServerReconnectLoop(thread, game, pipe);
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

    print("Got thread start address: " << (LPVOID)start_addr);
    CreateDetours();

    /**
    Game* game = Game::GetGame(start_addr);

    Packet packet = CreatePacket(game);

    print("Got sample packet");
    print(packet.length);
    print(packet.message);

    print("Starting server loop");
    int ret_value = GameServerLoop(thread, game);
    CloseHandle(thread);

    return ret_value;
    */

   
    CloseHandle(thread);


    return 0;
}
