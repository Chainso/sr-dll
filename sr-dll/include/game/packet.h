#pragma once
#ifndef GAME_PACKET_H
#define GAME_PACKET_H	

#include <string>

#include "game.h"

struct Packet
{
	size_t length;
	std::string message;
};

Packet CreatePacket(Game* game);

#endif // GAME_PACKET_H