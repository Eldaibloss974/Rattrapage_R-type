# Networked Multiplayer Game

## Project Purpose
This project is a networked multiplayer game built with SFML and UDP communication. Players connect to a server, interact with each other, and engage in real-time actions such as movement and shooting.

## Features
- Real-time multiplayer gameplay
- Player movement and shooting mechanics
- Monster spawning
- Synchronization of game state across clients

## Dependencies
- **Raylib**: A simple and fast multimedia library for graphics, windowing, audio, etc.
- **Boost**: Used for networking and threading.
- **C++17**: For language features and libraries.

## Supported Platforms
- Linux

## Installation and Build Instructions

### 1. Clone the repository:
   ```bash
   git clone git@github.com:EpitechPromo2027/B-CPP-500-TLS-5-2-rtype-enzo.dziewulski.git
   ```
## Build the project CMD:

```
    chmod +x ./r_type.sh
    sudo ./r_type.sh
```
### 4. Build the project using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

### 5. Run the game:

Start the server:

You can specify the server port by providing it as the first argument:

```bash
./r-type_server [port]
```
If no port is specified, the server will use the default port (8080).

Start a client:

You can specify the server address and optionally the port (default is 8080):

```bash
./r-type_client <server_address> [port]
```

Usage Instructions

1. Start the server:

The server handles all connections and game state synchronization. Start it first using the command:

```bash
./r-type_server
```

2. Start the clients:

After the server is running, start the client with a unique name:

```bash
./r-type_client
```

3. Gameplay:

    Movement: Use the arrow keys to move your character.
    Shooting: space to shoot.
    Multiplayer: You will see other players in the game world and interact with them in real-time.

## Protocol
### 1. Packet Structure:

All communication between the server and client is done through UDP packets. Each packet has a header followed by the command and data. The header contains:

    seq (uint16_t): Sequence number of the packet.
    ack (uint16_t): Acknowledgment number from the last received packet.
    flags (uint8_t): Flags for packet type (normal, ACK, etc.).
    reserved (uint8_t): Reserved space for future extensions.

The data that follows depends on the command being sent.
### 2. Commands:

The protocol includes the following commands, each associated with a specific action:

2.1 Client-to-Server Commands

| Command Code | Command Name         | Description                                          |
|--------------|----------------------|------------------------------------------------------|
| 0            | `CONNECT_PLAYER`    | Initiates connection with the player's ID.            |
| 1            | `DISCONNECT_PLAYER` | Requests player disconnection.                        |
| 2            | `CREATE_ROOM`       | Requests creation of a game room with details.        |
| 5            | `SEARCH_ROOM`       | Searches for available rooms.                         |
| 6            | `JOIN_ROOM`         | Joins a specific room with player and room details.   |
| 8            | `REQUEST_ROOMS`     | Requests a list of available rooms.                   |
| 9            | `SEND_INPUT`        | Sends player input (e.g., keypresses).                |
| 10           | `QUIT_GAME`         | Informs the server that the player has quit.          |

2.2 Server-to-Client Commands

| Command Code | Command Name             | Description                                              |
|--------------|--------------------------|----------------------------------------------------------|
| 0            | `ACCEPT_CONNECT_PLAYER` | Confirms player connection with their ID.                 |
| 1            | `NOT_ACCEPT_CONNECT_PLAYER` | Rejects connection attempt.                           |
| 2            | `ACCEPT_DISCONNECT_PLAYER` | Confirms player disconnection.                         |
| 3            | `ACCEPT_CREATE_ROOM`    | Confirms room creation with details.                      |
| 4            | `NOT_ACCEPT_CREATE_ROOM` | Rejects room creation with an error message.             |
| 5            | `RECEIVE_ROOMS`         | Sends the list of available rooms.                        |
| 6            | `JOIN_ROOM_SUCCESS`     | Confirms successful room joining.                         |
| 7            | `JOIN_ROOM_FAILURE`     | Reports failure to join a room.                           |
| 8            | `CREATE_ENTITY`         | Notifies creation of a game entity.                       |
| 9            | `DELETE_ENTITY`         | Notifies deletion of a game entity.                       |
| 10           | `POSITION_ENTITY`       | Updates position of a game entity.                        |
| 11           | `GAME_OVER`            | Notifies clients that the game has ended.                  |

### 3. Server Communication:

    The server listens for commands from clients.
    When a client sends its position, the server relays this to all other clients.
    When a client shoots, the server broadcasts the shoot command to all clients.
    The server handles the creation of monsters and relays the spawn information to clients.

### 4. Client Communication:

    The client sends commands like POSITION_PLAYER to the server.
    The client receives commands from the server, such as other players' positions or monster spawn events, and updates its game state accordingly.
