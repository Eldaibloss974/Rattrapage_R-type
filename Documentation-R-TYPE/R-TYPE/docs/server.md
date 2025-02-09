# R-TYPE Protocol (RFC 002 By Me)

## Introduction

Players must connect to the server before issuing commands. However, `DISCONNECT` and `HELP` can be used without prior connection.

## Mandatory R-TYPE Commands (Client to Server)

- `CONNECT <SP> <username> <CRLF>`  
  Connects the player to the server.

- `DISCONNECT <CRLF>`  
  Disconnects the player from the server.

- `LIST_PLAYERS <CRLF>`  
  Retrieves a list of all connected players.

- `READY <CRLF>`  
  Indicates the player is ready to start the game.

- `START_GAME <CRLF>`  
  Requests to start the game if all players are ready.

- `GAME_STATE <CRLF>`  
  Retrieves the current state of the game.

- `MOVE <SP> <direction> <CRLF>`  
  Sends a movement command to the server (`UP`, `DOWN`, `LEFT`, `RIGHT`).

- `SHOOT <CRLF>`  
  Fires a projectile.

- `SCORE_UPDATE <CRLF>`  
  Retrieves the current score of all players.

- `GAME_OVER <CRLF>`  
  Acknowledges the end of the game.

- `HELP <CRLF>`  
  Displays help information.

## Command-Reply Sequences

The server reply consists of:
- A **3-digit code**, followed by:
- A **space `<SP>`**, followed by:
- A **message**, terminated by `<CRLF>`.

### Example
```plaintext
-> CONNECT <SP> Player1 <CRLF>
<- 201 <SP> Welcome Player1! Connection successful. <CRLF>
<- 401 <SP> Connection failed. Server full. <CRLF>
