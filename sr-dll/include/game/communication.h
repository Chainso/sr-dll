#pragma once
#ifndef GAME_COMMUNICATION_H
#define GAME_COMMUNICATION_H

#include <unordered_map>

#include "game/game.h"
#include "game/structs.h"

#define WRITE_BUFFER_SIZE 1024
#define READ_BUFFER_SIZE 1024


namespace Communication
{
    struct IsPressedArg {
        char pad_0[8];
        wchar_t action[0]; // UTF-16 null terminated string
    };

    class Communicator;

    extern Communicator* comm;

    // Detours
    bool __fastcall IsPressed(void* arg, IsPressedArg* pressed_arg);
    bool __fastcall GetInputAndCheckPressed(void* arg, IsPressedArg* pressed_arg);

    class Communicator
    {
    private:
        Game* game;

        LPCWSTR pipe_name;
        HANDLE pipe;
        bool connected = false;
        PlayerInput controller = { 0 };

        const std::unordered_map<std::wstring, bool*> actionTable = {
            {L"ps_left", &controller.left},
            {L"ps_right", &controller.right},
            {L"ps_jump", &controller.jump},
            {L"ps_grapple", &controller.grapple},
            {L"ps_weapon", &controller.weapon},
            {L"ps_item", &controller.item},
            {L"ps_taunt", &controller.taunt},
            {L"ps_swap", &controller.swap_weapon},
            {L"ps_slide", &controller.slide},
            {L"ps_boost", &controller.boost}
        };

        char read_buffer[READ_BUFFER_SIZE];
        DWORD bytes_read;
        DWORD bytes_written;

        Communicator(Game* game, LPCWSTR pipe_name) : game(game), pipe_name(pipe_name) {}

    public:
        static Communicator* CreateCommunicator(Game* game, LPCWSTR pipe_name);
        bool CreatePipe();
        bool ConnectAndSetupPipe();
        bool DisconnectPipe();
        bool SendPacket();
        bool ReceivePacket();
        bool* ParseAction(wchar_t* action);
    };
}

#endif // GAME_COMMUNICATION_H