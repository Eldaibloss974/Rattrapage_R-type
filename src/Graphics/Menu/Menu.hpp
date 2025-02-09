/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Menu.hpp
*/

#pragma once

#include "../InitialisedWorld.hpp"
#include <boost/lockfree/queue.hpp>
#include <vector>
#include <string>
#include "raylib.h"
#include "TextureManager.hpp"

using PacketHandler = std::function<void(sf::Packet&)>;

class Menu {
    public:
        struct Room {
            std::string name;
            int level;
            int players;
            Rectangle rect;
        };

        enum KEYS {
            KEY_RIGHT = 0,
            KEY_LEFT = 1,
            KEY_UP = 2,
            KEY_DOWN = 3,
            KEY_SPACE = 4,
            KEY_UNKNOWN = 5
        };

        Menu(Client &client);
        ~Menu();

        TextureManager &getTextureManager() { return textureManager_; }

        bool isConnected() const;
        void handleInput(float deltaTime);
        void update();
        void draw();
        void handlePacket(boost::lockfree::queue<sf::Packet*>& packetQueue_, bool &isRunning_);

        void disconnectClient();

        // CREATE ROOM
        void drawCreateRoomScreen();
        void handleRoomCreationInput();

        // SEARCH ROOM
        std::vector<Room> rooms_;
        int scrollOffset_;
        bool showSearchRoomScreen_ = false;
        void requestRooms();
        void drawSearchRoomScreen();
        void handleSearchRoomInput();

        // OPTION
        Sound menuSound_;
        bool showOptionsScreen_ = false;
        int volumeLevel_;
        bool mute_;
        std::string serverHost_ = "localhost";
        int serverPort_ = 2121;
        void drawOptionsScreen();
        void handleOptionsInput();

        // GAME
        bool isInGame = false;
        InitialiseWorld world_;

        Client &client_;

        // Choose Skin Player
        bool showSkinScreen_ = false;
        Texture2D texture_;
        std::vector<Texture2D> skinsTextures_;
        std::vector<std::string> skins_;
        int selectedSkinIndex_;
        std::string skinPath_;
        void drawSkinScreen();
        void handleSkinInput();
        float rotationAngle_;

    private:
        void drawLoginScreen();
        void drawMainMenu();
        void connectClient();
        void handleTextInput();

        struct Button {
            Rectangle rect;
            std::string text;
            bool hovered;

            void draw();
        };

        bool connected_;

        bool inputFieldActive_;
        bool isEditingRoomName_ = true;

        bool showLoginScreen_ = true;
        bool showCreateRoomScreen_ = false;
        bool showMainMenu_ = false;

        TextureManager textureManager_;
        std::string clientId_;
        std::string password_;
        std::vector<Button> buttons_;

        std::string errorMessage_;

        int roomLevel_;
        std::string roomName_;

        void clearErrorMessage();

        // Commande Handler
        std::map<sf::Uint8, PacketHandler> packetHandlers_;

        void initializePacketHandlers();

        void handleAcceptConnectPlayer(sf::Packet &packet);
        void handleAcceptDisconnectPlayer(sf::Packet &packet);
        void handleNotAcceptConnectPlayer(sf::Packet &packet);
        void handleAcceptCreateRoom(sf::Packet &packet);
        void handleNotAcceptCreateRoom(sf::Packet &packet);
        void handleReceiveRooms(sf::Packet &packet);
        void handleJoinRoomSuccess(sf::Packet &packet);
        void handleJoinRoomFailure(sf::Packet &packet);
        void handleCreateEntity(sf::Packet &packet);
        void handlePositionEntity(sf::Packet &packet);
        void handleDeleteEntity(sf::Packet &packet);
        void handleGameOver(sf::Packet &packet);
        void handleScorePlayer(sf::Packet &packet);

        void processPacket(sf::Packet &packet);

        bool isRunning_;
};
