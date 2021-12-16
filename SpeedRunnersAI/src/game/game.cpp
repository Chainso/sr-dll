#include "game/game.h"
#include "game/offsets.h"
#include "game/memory.h"

/**
 * @brief Gets the running game instance using the first thread in the stack.
 * 
 * @param first_thread	The first thread of the process.
 * @return Game* 		The game instance to return.
 */
Game* Game::GetGame(uintptr_t first_thread)
{

	// TODO: use addresses to lookup game info and players
	int initialOff = -0x994;
	std::vector<unsigned int> offsets = { 0x8, 0x50, 0xC, 0x124, 0x20, 0x4, 0xC, 0x3C };

	GameInfo* info = (GameInfo*)FindAddress(first_thread + AddressOffsets::game.base, AddressOffsets::game.offsets);
	std::vector<Player*> players;
	players.push_back((Player*)FindAddress(first_thread + AddressOffsets::player.base, AddressOffsets::player.offsets));

	Game* game = new Game(info, players);

	return game;
}