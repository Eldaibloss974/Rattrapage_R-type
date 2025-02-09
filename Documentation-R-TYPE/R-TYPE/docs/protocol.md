# Protocol

## 1. Introduction

This document outlines the updated communication protocol for the networked game. It specifies the structure and commands used for message exchanges between the server and the client. This protocol is essential for maintaining game state synchronization and enabling smooth interaction between players.

The protocol utilizes data packets composed of a header and payload. These packets are exchanged between the client and server for various game actions, such as player movement, room management, and game events.

## 2. Terminology and Assumptions

- **Client**: A program running the game, representing a player.
- **Server**: A program hosting the game, managing game state and client synchronization.
- **Packet**: A structured unit of communication containing a header and payload.
- **Sequence Number (seq)**: A unique identifier for packets to ensure order and delivery.
- **Acknowledgment (ack)**: A field in packets for confirming receipt of prior packets.

## 3. Packet Structure

Each packet consists of:
- **Header**: Metadata for delivery, order, and validation.
- **Payload**: Command-specific data.

### 3.1 Packet Header

The header has the following structure:

| Field    | Type      | Description                                  |
|----------|-----------|----------------------------------------------|
| `seq`    | uint16_t  | Sequence number for packet order.            |
| `ack`    | uint16_t  | Last received packet's sequence number.      |
| `flags`  | uint8_t   | Packet type flags (e.g., data, ack).         |
| `reserved` | uint8_t | Reserved for future extensions (fixed to 0). |

**Header size**: 8 bytes.

### 3.2 Packet Payload

The payload contains:
- Command identifier (numeric code).
- Command-specific data.

## 4. Commands

Commands are categorized into client-to-server and server-to-client messages.

### 4.1 Client-to-Server Commands

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

### 4.2 Server-to-Client Commands

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

## 5. Communication Flow

### 5.1 Client Sends Player Input

The client sends player actions or input using the `SEND_INPUT` command.

Example:
```cpp
sf::Packet packet;
packet << SEND_INPUT << playerId << key;
server.send(packet);
```

5.2 Server Acknowledges Connection

The server confirms a player's connection using the `ACCEPT_CONNECT_PLAYER` command.

Example:
```cpp
sf::Packet packet;
packet << ACCEPT_CONNECT_PLAYER << clientId_;
client.send_to_client(clientEndpoint, PacketToString(packet));
```

### 5.3 Room Management

- **Creating a Room**: The client sends a `CREATE_ROOM` command with room details. The server responds with `ACCEPT_CREATE_ROOM` or `NOT_ACCEPT_CREATE_ROOM`.
- **Joining a Room**: The client sends a `JOIN_ROOM` command. The server responds with `JOIN_ROOM_SUCCESS` or `JOIN_ROOM_FAILURE`.

## 6. Sequence Numbers and Acknowledgments

Sequence numbers and acknowledgment fields ensure reliable communication and ordered packet delivery.

- **Sequence Numbers**: Each packet includes a unique sequence number.
- **Acknowledgments**: Sent by the receiver to confirm received packets.

## 7. Error Handling and Retransmission

If a packet is lost or delayed:
- The sender resends the packet after a timeout.
- The receiver uses the acknowledgment system to request retransmission.

## 8. Example Interaction

- **Player Connection**: The client sends `CONNECT_PLAYER`. The server replies with `ACCEPT_CONNECT_PLAYER`.
- **Player Action**: The client sends `SEND_INPUT` with player actions. The server processes and broadcasts updates.
- **Game End**: The server sends `GAME_OVER` to notify all clients.

## 9. Conclusion

This protocol provides a robust framework for communication between the client and server. It enables reliable synchronization of game states and smooth interaction between players. By following this specification, developers can implement compatible clients and servers for the game.

Appendix: Command Reference

### Client-to-Server Commands
- `CONNECT_PLAYER (0)`
- `DISCONNECT_PLAYER (1)`
- `CREATE_ROOM (2)`
- `SEARCH_ROOM (5)`
- `JOIN_ROOM (6)`
- `REQUEST_ROOMS (8)`
- `SEND_INPUT (9)`
- `QUIT_GAME (10)`

### Server-to-Client Commands
- `ACCEPT_CONNECT_PLAYER (0)`
- `NOT_ACCEPT_CONNECT_PLAYER (1)`
- `ACCEPT_DISCONNECT_PLAYER (2)`
- `ACCEPT_CREATE_ROOM (3)`
- `NOT_ACCEPT_CREATE_ROOM (4)`
- `RECEIVE_ROOMS (5)`
- `JOIN_ROOM_SUCCESS (6)`
- `JOIN_ROOM_FAILURE (7)`
- `CREATE_ENTITY (8)`
- `DELETE_ENTITY (9)`
- `POSITION_ENTITY (10)`
- `GAME_OVER (11)`
