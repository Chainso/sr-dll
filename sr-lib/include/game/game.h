#pragma once
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <vector>

#include "structs.h"

class Game
{
public:
	GameInfo* info;
	std::vector<Player*> players;

	Game(GameInfo* gi, std::vector<Player*> v_players) : info(gi), players(v_players) {}
	static Game* GetGame(uintptr_t first_thread);
};

#endif // GAME_GAME_H