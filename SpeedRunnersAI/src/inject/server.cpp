#include <windows.h>

#include "inject/server.h"
#include "inject/thread.h"
#include "inject/ntinfo.h"
#include "game/game.h"

#include <fstream>

/**
 * @brief The main game server loop.
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
