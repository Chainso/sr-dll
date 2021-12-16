#pragma once

#ifndef GAME_STRUCTS_HPP
#define GAME_STRUCTS_HPP

#include <stdint.h>

struct Vector2
{
	float x;
	float y;
};


struct Entity
{
	char pad_0000[60]; //0x0000
	Vector2 position; //0x003C
	char pad_0044[16]; //0x0044
	Vector2 velocity; //0x0054
};

struct Player
{
	char pad_0000[36]; //0x0000
	Entity* entity; //0x0024
	char pad_0028[508]; //0x0028
	float grapple_radius; //0x0224
	char pad_0228[16]; //0x0228
	float boost; //0x0238
	char pad_023C[32]; //0x023C
	float lap_time; //0x025C
	char pad_0260[48]; //0x0260
	uint8_t sliding; //0x0290
	uint8_t unknown; //0x0291
	uint8_t swinging; //0x0292
	char pad_0293[10]; //0x0293
	uint8_t inAir; //0x029D
	char pad_029E[18]; //0x029E
	uint8_t onGround; //0x02B0
	char pad_02B1[15]; //0x02B1
	uint8_t item; //0x02C0
	char pad_02C1[71]; //0x02C1
	float last_move_direction; //0x0308
};

struct GameInfo
{
	char pad_0000[120]; //0x0000
	float lap_time; //0x0078
};



#endif // GAME_STRUCTS_H