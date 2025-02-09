# Developer Documentation

## 2. Developer Documentation

This section aims to help new developers dive into your codebase.
### 2.1 Project Architecture
Provide a high-level overview of the project's architecture. It can include a diagram showing the communication between the client, server, and game logic.

Client: Responsible for rendering the game, handling user input, and communicating with the server.
Server: Manages the game state, handles networking, and synchronizes actions across clients.
Game Logic: Handles gameplay, such as player actions, NPC behavior, and game events.

Diagram:
CopyClient ↔ Server (UDP communication)
Server ↔ Game Logic (State management)

### 2.2 Key Components and Their Roles

Game: Manages the main game loop, handles player actions, sends/receives packets from the server.
Networking: Handles the UDP communication between client and server, including packet serialization/deserialization.
Player: Represents a player in the game world. Handles movement, shooting, and health.
Projectile: Represents a projectile fired by a player. Includes logic for updating its position and checking if it hits an enemy.
Monster: Spawns in the game world. Has basic AI behavior.

### 2.3 Contribution Guidelines

Code Style: Follow the C++11/17 standard, use meaningful variable names, and comment complex sections.
Commit Messages: Use clear and concise commit messages, e.g., fix: correct player movement sync issue.
Pull Requests: Fork the repository, make changes in a new branch, and submit a pull request.

## 3. Algorithms and Data Structures

Networking (UDP vs TCP): UDP is used because it's faster and more efficient for real-time communication in multiplayer games. While TCP guarantees delivery, its overhead isn't suitable for fast-paced games where low latency is crucial.
Data Structures: A hash map is used to store players by their ID (using std::unordered_map). This provides quick lookups for actions involving players.

### 3.1 Storage and Persistence

Game State: Game state (positions, health, etc.) is stored in memory. If the game state needs to be saved for later, we could use a database or flat files. However, for real-time multiplayer games, persisting state between sessions isn't required unless saving the game world is a feature.
Session Data: Sessions are managed by the server, which does not store data permanently unless explicitly required.

### 3.2 Security

Data Integrity: UDP packets are prone to loss, so a simple sequence numbering and acknowledgment mechanism ensures that packets are received in order and are not lost.
Security Concerns: While the game uses UDP, there is minimal encryption or validation of the data, which could be a potential vulnerability. In future work, securing communications via SSL/TLS or implementing basic encryption could be beneficial.