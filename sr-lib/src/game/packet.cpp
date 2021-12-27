#include "game/packet.h"
#include "json.hpp"

static void to_json(nlohmann::json& j, const GameInfo* info)
{
	j = nlohmann::json{
		{"lap_time", info->lap_time}
	};
}

static void to_json(nlohmann::json& j, const Vector2& vec)
{
	j = nlohmann::json{
		{"x", vec.x},
		{"y", vec.y}
	};
}
 
static void to_json(nlohmann::json& j, const PlayerInput& input)
{
	j = nlohmann::json{
		{"left", input.left},
		{"right", input.right},
		{"jump", input.jump},
		{"grapple", input.grapple},
		{"weapon", input.weapon},
		{"item", input.item},
		{"taunt", input.taunt},
		{"swap_weapon", input.swap_weapon},
		{"slide", input.slide},
		{"boost", input.boost}
	};
}

static void from_json(const nlohmann::json& j, PlayerInput& input)
{
	j.at("left").get_to(input.left);
	j.at("right").get_to(input.right);
	j.at("jump").get_to(input.jump);
	j.at("grapple").get_to(input.grapple);
	j.at("weapon").get_to(input.weapon);
	j.at("item").get_to(input.item);
	j.at("taunt").get_to(input.taunt);
	j.at("swap_weapon").get_to(input.swap_weapon);
	j.at("slide").get_to(input.slide);
	j.at("boost").get_to(input.boost);
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
		{"input", player->input},
		{"in_air", player->in_air},
		{"sliding", player->sliding},
		{"sliding_on_ground", player->sliding},
		{"grappling", player->grappling},
		{"on_wall", player->on_wall},
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
 * @param game  	The game state to create the packet from.
 * @return Packet   The packet object from the game state.
 */
Packet CreatePacket(Game* game)
{
	nlohmann::json message = game;

	Packet packet;
	packet.message = message.dump();
	packet.length = packet.message.size();

	return packet;
}


/**
 * @brief Parses a packet into the player input.
 * 
 * @param buffer		The buffer containing the message to parse.
 * @param len 			The length of the buffer.
 * @return PlayerInput 	The parsed packet.
 */
PlayerInput ParsePacket(char* buffer, size_t len)
{
	nlohmann::json message = nlohmann::json::parse(buffer, buffer + len);
	PlayerInput packet = message;

	return packet;
}