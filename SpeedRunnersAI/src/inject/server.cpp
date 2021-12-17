#include <windows.h>

#include "inject/server.h"
#include "inject/thread.h"
#include "inject/ntinfo.h"
#include "game/game.h"
#include "game/packet.h"

#include <fstream>
#include <chrono>
#include <thread>


/**
 * @brief The main game server loop.
 * 
 * @param game  The current running game.
 * @return      0 on success, -1 otherwise.
 */
static int GameServerLoop(Game* game)
{
    // Create named pipe to use
    LPCWSTR pipe_name = L"SpeedRunnersAI-dll";
    
    HANDLE pipe = CreateNamedPipe(pipe_name, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_NOWAIT | PIPE_REJECT_REMOTE_CLIENTS, 1, WRITE_BUFFER_SIZE, READ_BUFFER_SIZE, 0, NULL);

    if (pipe == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    // Wait for client to connect
    if (ConnectNamedPipe(pipe, NULL) == 0)
    {
        // Failed to connect the client
        return -1;
    }

    char read_buffer[READ_BUFFER_SIZE];
    DWORD bytes_read;

    DWORD bytes_written;

    while (true)
    {
        // Read cycle
        if (PeekNamedPipe(pipe, read_buffer, READ_BUFFER_SIZE, &bytes_read, NULL, NULL))
        {
            // Data is available
            if (!ReadFile(pipe, read_buffer, READ_BUFFER_SIZE, &bytes_read, NULL))
            {
                // Error reading
                return -1;
            }
        }

        // Write cycle
        Packet packet = CreatePacket(game);
        WriteFile(pipe, packet.message.c_str(), packet.length, &bytes_written, NULL);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Disconnect from client
    if (DisconnectNamedPipe(pipe) == 0)
    {
        // Failed to disconnect
        return -1;
    }

    return 0;
}

/**
 * @brief Starts the game server to receive inputs and export packets.
 * 
 * @return int  The return status of the server, -1 on error.
 */
int GameServer()
{
    HANDLE thread = GetFirstThread();
    
    if (thread == INVALID_HANDLE_VALUE) {
        return -1;
    }

    HANDLE process = GetCurrentProcess();

    uintptr_t start_addr = (uintptr_t)GetThreadStartAddress(process, thread);

    CloseHandle(process);
    CloseHandle(thread);

    std::ofstream fs("C:\\Users\\Chainso\\Desktop\\zz.txt");

    fs << (LPVOID)start_addr << std::endl;

    Game* game = Game::GetGame(start_addr);

    fs << (LPVOID)game->info << " " << game->info->lap_time << std::endl;

    for (Player*& player : game->players) {
        fs << (LPVOID)player << " " << player->entity->position.x << " " << player->entity->position.y << std::endl;
    }

    Packet packet = CreatePacket(game);

    fs << packet.length << std::endl;
    fs << packet.message << std::endl;

    fs.close();
	return 0;
}
