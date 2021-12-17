#include <windows.h>

#include "inject/server.h"
#include "inject/thread.h"
#include "inject/ntinfo.h"
#include "game/game.h"

#include <fstream>

/**
 * @brief Creates a packet from the current game state.
 * 
 * @param game  The game state to create the packet from.
 * @returns     The create packet as a string
 */
static std::string CreatePacket(Game* game)
{

}

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
    
    HANDLE pipe = CreateNamedPipe(pipe_name, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_REJECT_REMOTE_CLIENTS, 1, WRITE_BUFFER_SIZE, READ_BUFFER_SIZE, 0, NULL);

    if (pipe == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    char read_buffer[READ_BUFFER_SIZE];
    DWORD bytes_read;

    char write_buffer[WRITE_BUFFER_SIZE];
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
        //WriteFile(pipe, write_buffer, -1, &bytes_written, NULL);
    }
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

    fs.close();
	return 0;
}
