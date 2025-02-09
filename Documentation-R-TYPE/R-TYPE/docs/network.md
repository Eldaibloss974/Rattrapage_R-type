# Network

## Network Implementation: Why Asio?

For the networking layer of the game, we chose to use Asio, a powerful C++ library designed for asynchronous input/output operations. Asio is a well-established library widely used for network programming and provides robust support for both synchronous and asynchronous communication. Below, we explain why Asio was the library of choice for this project and how it aligns with the requirements of a real-time multiplayer game.
Why Asio?

    Asynchronous Operations
    Asio's asynchronous model is essential for a real-time game, where performance is critical, and operations like sending/receiving data must not block the main game loop.
        Asynchronous I/O ensures smooth gameplay by allowing network operations to run in the background while the game processes other tasks such as rendering and input handling.

    Cross-Platform Compatibility
    Asio is platform-agnostic and works seamlessly across Linux, Windows, and macOS. This was a crucial factor in ensuring that our game could be built and deployed on multiple systems without significant modifications.

    UDP Support
    For our game, we required a fast and lightweight protocol to manage real-time updates, such as player movements, shooting actions, and monster spawning. UDP (User Datagram Protocol) was the natural choice due to its low latency.
        Asio provides excellent support for UDP communication while allowing us to handle packet delivery reliability using custom sequence and acknowledgment numbers.

    Thread Management
    Asio provides tools for managing threads and handling concurrent tasks efficiently. In our case, it allowed us to handle multiple client connections simultaneously, ensuring smooth synchronization of game states.

    Ease of Integration
    The library integrates seamlessly with modern C++ (C++11 and above), leveraging features like lambdas and smart pointers. This reduced development complexity and allowed us to write clean, maintainable code.

    Documentation and Community Support
    Asio has comprehensive documentation and an active community. This made it easier to troubleshoot issues and understand advanced features when implementing the networking layer.

## How Asio is Used in the Project

    Server-Side Networking
    On the server, Asio manages:
        Listening for client connections: The server binds to a specific UDP port and continuously listens for incoming messages from clients.
        Broadcasting updates: The server sends updates (e.g., player positions, actions, or monster spawns) to all connected clients.
        Managing multiple clients: Asio's support for multithreading enables efficient handling of multiple client connections.

    Client-Side Networking
    On the client, Asio is used to:
        Send player actions: Clients send packets to the server containing movement, shooting actions, and other events.
        Receive server updates: The client processes packets from the server, synchronizing its local game state with the server's authoritative state.

    Custom Protocol Handling
        We implemented our custom packet structure, including headers for sequence numbers and acknowledgments, ensuring ordered and reliable delivery despite using UDP.
        Asio provided the flexibility to implement this logic while handling low-level socket operations efficiently.

## Comparison with Other Libraries

We considered several alternatives before settling on Asio:
Library	Strengths	Weaknesses
Asio	Lightweight, asynchronous, cross-platform	Steeper learning curve for beginners
Boost.Asio	Includes Asio with additional Boost tools	Requires linking to the full Boost suite
Enet	Built-in reliability layer for UDP	Less flexible, less active community
SFML	Simple and easy-to-use networking module	Limited features for advanced use cases

Among these, Asio struck the right balance between performance, flexibility, and control over the network layer.
Conclusion

Asio proved to be an ideal choice for the networking requirements of this project. Its asynchronous I/O model, UDP support, and robust threading capabilities allowed us to build a responsive and scalable multiplayer game. By leveraging Asio, we achieved a low-latency communication system that synchronized game states efficiently while maintaining a smooth gameplay experience for all connected players.