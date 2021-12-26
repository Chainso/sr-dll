#pragma once

#ifndef GAME_STRUCTS_HPP
#define GAME_STRUCTS_HPP

#include <stdint.h>

struct Vector2
{
	float x;
	float y;
};

struct PlayerInput
{
	bool left; //0x0000
	bool right; //0x0001
	bool jump; //0x0002
	bool grapple; //0x0003
	char weapon; //0x0004 different source than the other inputs, perhaps for single player only
	bool item; //0x0005
	bool taunt; //0x0006
	bool swap_weapon; //0x0007
	char pad_0008[1]; //0x0008 seems to be unused in disassembler
	bool slide; //0x0009
	bool boost; //0x000A
}; //Size: 0x000B

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
	float grapple_angle; //0x0228
	char pad_022C[12]; //0x022C
	float boost; //0x0238
	char pad_023C[32]; //0x023C
	float lap_time; //0x025C
	char pad_0260[47]; //0x0260
	PlayerInput input; //0x028F
	char pad_029A[3]; //0x029A
	bool in_air; //0x029D
	char pad_029E[2]; //0x029E
	bool sliding; //0x02A0
	char pad_02A1[1]; //0x02A1
	bool sliding_on_ground; //0x02A2
	char pad_02A3[4]; //0x02A3
	bool grappling; //0x02A7
	char pad_02A8[4]; //0x02A8
	bool on_wall; //0x02AC
	char pad_02AD[3]; //0x02AD
	bool on_ground; //0x02B0
	char pad_02B1[19]; //0x02B1
	uint8_t item; //0x02C4
	char pad_02C5[71]; //0x02C5
	float last_move_direction; //0x030C
};

struct GameInfo
{
	char pad_0000[120]; //0x0000
	float lap_time; //0x0078
};



#endif // GAME_STRUCTS_H