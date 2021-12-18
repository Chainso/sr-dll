#include "game/packet.h"
#include "json.hpp"

static void to_json(nlohmann::json& j, const GameInfo* info)
{
	j = nlohmann::json{
		{"lap_time", info->lap_time}
	};
}

static void to_json(nlohmann::json& j, const Vector2 vec)
{
	j = nlohmann::json{
		{"x", vec.x},
		{"y", vec.y}
	};
}

static void to_json(nlohmann::json& j, const Entity* entity)
{
	j = nlohmann::json{
		{"position", entity->position},
		{"velocity", entity->velocity}
	};
}

static void to_json(nlohmann::json& j, const Player* player)
{
	j = nlohmann::json{
		{"entity", player->entity},
		{"grapple_radius", player->grapple_radius},
		{"grapple_angle", player->grapple_angle},
		{"boost", player->boost},
		{"lap_time", player->lap_time},
		{"sliding", player->sliding},
		{"swinging", player->swinging},
		{"in_air", player->in_air},
		{"on_ground", player->on_ground},
		{"item", player->item},
		{"last_move_direction", player->last_move_direction}
	};
}

static void to_json(nlohmann::json& j, const Game* game)
{
	j = nlohmann::json{
		{"info", game->info},
		{"players", game->players}
	};
}

/**
 * @brief Creates a packet from the current game state.
 *
 * @param game  The game state to create the packet from.
 * @returns     The packet object from the game state.
 */
Packet CreatePacket(Game* game)
{
	nlohmann::json message = game;

	Packet packet;
	packet.message = message.dump();
	packet.length = packet.message.size();

	return packet;
}