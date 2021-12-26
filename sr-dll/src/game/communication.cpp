#include <atlstr.h>

#include "game/communication.h"
#include "game/packet.h"
#include "util.h"

using namespace Communication;

Communicator* Communication::comm = NULL;

/**
 * @brief Returns true if the action is pressed in the local controller.
 *
 * @param arg           A dummy argument.
 * @param pressed_arg   The structure that contains the action as a UTF-16 string.
 * @return              True if the local controller is true for that action.
 */
bool __fastcall Communication::IsPressed(void* arg, IsPressedArg* pressed_arg)
{
    bool* action = comm->ParseAction(pressed_arg->action);

    if (action == NULL) {
        return false;
    }

    return *action;
}


/**
 * @brief First sends out the game packet, and waits for input, then checks if the input action is pressed.
 *
 * @param arg           A dummy argument.
 * @param pressed_arg   The structure that contains the action as a UTF-16 string.
 * @return              True if the action is pressed, false otherwise.
 */
bool __fastcall Communication::GetInputAndCheckPressed(void* arg, IsPressedArg* pressed_arg)
{
    if (!comm->SendPacket() || !comm->ReceivePacket())
    {
        // Problem sending or receiving packet, should try to fix the connection
        // Disconnect from client
        comm->DisconnectPipe();

        if (!comm->ConnectAndSetupPipe())
        {
            // issue reconnecting pipe, figure out what to do here
        }

        return false;
    }

    return IsPressed(arg, pressed_arg);
}


/**
 * @brief Creates the communicator singleton.
 *
 * @return  The existing communicator if already created, or a newly created one.
 */
Communicator* Communication::Communicator::CreateCommunicator(Game* game, LPCWSTR pipe_name)
{
    if (comm == NULL) {
        comm = new Communicator(game, pipe_name);
    }

    return comm;
}


/**
 * @brief Creates the pipe for communication.
 * 
 * @return  True if the pipe was created succesfully.
 */
bool Communication::Communicator::CreatePipe()
{
    // Create named pipe to use
    pipe = CreateNamedPipe(
        pipe_name,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
        1,
        WRITE_BUFFER_SIZE,
        READ_BUFFER_SIZE,
        0,
        NULL
    );

    if (pipe == INVALID_HANDLE_VALUE)
    {
        print_error("Failed to create named pipe: " << CW2A(pipe_name));
        return false;
    }

    return true;
}

/**
 * @brief Starts a connection to the pipe and sets up the pipe handle state.
 *
 * @return      True if the connection was created successfully.
 */
bool Communication::Communicator::ConnectAndSetupPipe()
{
    // Set the pipe to wait on messages
    DWORD pipe_mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    if (SetNamedPipeHandleState(pipe, &pipe_mode, NULL, NULL) == 0)
    {
        print_error("Failed to set pipe state to wait");

        DisconnectPipe();

        return false;
    }

    // Wait for client to connect
    print("Waiting for client...");
    if (ConnectNamedPipe(pipe, NULL) == 0)
    {
        // Failed to connect the client
        print_error("Failed to connect to client");
        return false;
    }

    print("Connected to client!");

    if (SetNamedPipeHandleState(pipe, &pipe_mode, NULL, NULL) == 0)
    {
        print_error("Failed to set pipe state to wait");

        DisconnectPipe();

        return false;
    }

    return true;
}

/**
 * @brief Disconnects the pipe.
 *
 * @return  True if the pipe was disconnected successfully.
 */
bool Communication::Communicator::DisconnectPipe()
{
    if (DisconnectNamedPipe(pipe) == 0)
    {
        // Failed to disconnect
        print_error("Failed to disconnect pipe");
        return false;
    }

    return true;
}

/**
 * @brief Sends a packet out on the pipe.
 *
 * @return  True if the packet was sent successfully.
 */
bool Communication::Communicator::SendPacket()
{
    Packet packet = CreatePacket(game);

    if (!WriteFile(pipe, packet.message.c_str(), packet.length, &bytes_written, NULL))
    {
        print_error("Failed to write to pipe, disconnecting");
        return false;
    }

    if (bytes_written > 0)
    {
        print("Sent packet");
        print("Length: " << packet.length);
        print(packet.message);
    }
    return true;
}

/**
 * @brief Receives and processes a packet from the pipe.
 *
 * @return  True if the packet was received successfully, and the connection still remains.
 */
bool Communication::Communicator::ReceivePacket()
{
    if (ReadFile(pipe, read_buffer, READ_BUFFER_SIZE, &bytes_read, NULL))
    {
        controller = ParsePacket(read_buffer, bytes_read);
        return true;
    }
    else if (GetLastError() == ERROR_HANDLE_EOF)
    {
        // EOF, exit out
        print("Read EOF, disconnecting");
    }
    else if (GetLastError() != ERROR_NO_DATA)
    {
        // Read failed, and not because of no data
        print_error("Failed to read from pipe, disconnecting");
    }

    return false;
}


/**
 * @brief Parses the action into a pointer containing the current value of the action.
 *
 * @return  The pointer to the current value of the action, or NULL if the action is invalid.
 */
bool* Communication::Communicator::ParseAction(wchar_t* action)
{
    std::wstring act_str(action);
    auto act_it = actionTable.find(act_str);

    if (act_it == actionTable.end())
    {
        // Invalid action
        std::wcout << "Invalid action: " << act_str << std::endl;
        return NULL;
    }

    return act_it->second;
}