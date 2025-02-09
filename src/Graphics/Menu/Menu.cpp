/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Menu.cpp
*/

#include "Menu.hpp"
#include <iostream>

Menu::Menu(Client &client) : client_(client), connected_(false), inputFieldActive_(true), errorMessage_(""), roomLevel_(1), world_(client)
{
    buttons_.push_back({{300, 200, 200, 50}, "Search Room", false});
    buttons_.push_back({{300, 300, 200, 50}, "Create Room", false});
    buttons_.push_back({{300, 400, 200, 50}, "Options", false});
    buttons_.push_back({{300, 500, 200, 50}, "Quit", false});
    buttons_.push_back({{650, 550, 150, 40}, "Skin", false});

    skins_ = {
        "../assets/sprites/green_player.gif",
        "../assets/sprites/red_player.gif",
        "../assets/sprites/purple_player.gif",
        "../assets/sprites/blue_player.gif",
        "../assets/sprites/blue_player2.gif"
    };
    skinPath_ = skins_[0];
    selectedSkinIndex_ = 0;

    showOptionsScreen_ = false;
    volumeLevel_ = 50;
    mute_ = false;
    menuSound_ = LoadSound("../assets/song.mp3");
    PlaySound(menuSound_);
    SetSoundVolume(menuSound_, volumeLevel_ / 100.0f);
    serverPort_ = 2121;
    isInGame = false;

    initializePacketHandlers();
}

Menu::~Menu()
{
    UnloadSound(menuSound_);
}

bool Menu::isConnected() const
{
    return connected_;
}

void Menu::handleInput(float deltaTime)
{
    // GAME ////
    if (isInGame) {
        world_.updates(deltaTime);
        return;
    }

    if (showSkinScreen_) {
        handleSkinInput();
        return;
    }

    if (showLoginScreen_) {
        if (inputFieldActive_) {
            handleTextInput();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            connectClient();
        }
    } else if (showCreateRoomScreen_) {
        handleRoomCreationInput();
    } else if (showSearchRoomScreen_) {
        handleSearchRoomInput();
    } else if (showOptionsScreen_) {
        handleOptionsInput();
    } else {
        Vector2 mousePos = GetMousePosition();
        for (auto &button : buttons_) {
            button.hovered = CheckCollisionPointRec(mousePos, button.rect);
            if (button.hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (button.text == "Quit") {
                    errorMessage_.clear();
                    disconnectClient();
                    isRunning_ = false;
                } else if (button.text == "Search Room") {
                    requestRooms();
                    errorMessage_.clear();
                    showSearchRoomScreen_ = true;
                    std::cout << "Search Room button clicked!" << std::endl;
                } else if (button.text == "Create Room") {
                    errorMessage_.clear();
                    showCreateRoomScreen_ = true;
                } else if (button.text == "Options") {
                    errorMessage_.clear();
                    showOptionsScreen_ = true;
                    std::cout << "Options button clicked!" << std::endl;
                } else if (button.text == "Skin") {
                    showSkinScreen_ = true;
                }
            }
        }
    }
}

void Menu::update() {}

void Menu::draw()
{
    if (client_.returnToMenu_) {
        return;
    }

    if (isInGame) {
        return;
    }
    if (showSkinScreen_) {
        drawSkinScreen();
    } else if (showSearchRoomScreen_) {
        drawSearchRoomScreen();
    } else if (showLoginScreen_) {
        drawLoginScreen();
    } else if (showCreateRoomScreen_) {
        drawCreateRoomScreen();
    } else if (showOptionsScreen_) {
        drawOptionsScreen();
    } else {
        drawMainMenu();
    }
}

/////////////////////////////////////////////// LOGIN ////////////////////////////////////////////////////////
void Menu::drawLoginScreen()
{
    DrawText("Enter Client ID", 250, 150, 20, WHITE);
    DrawRectangle(250, 200, 300, 30, inputFieldActive_ ? DARKGRAY : GRAY);
    DrawText(clientId_.c_str(), 205, 205, 20, WHITE);
    DrawText("Press ENTER to connect", 250, 250, 20, WHITE);
    if (!errorMessage_.empty()) {
        DrawText(errorMessage_.c_str(), 250, 300, 20, RED);
    }
}

void Menu::connectClient()
{
    if (!clientId_.empty()) {
        sf::Packet packet;
        packet << CONNECT_PLAYER << clientId_;
        client_.send_message(packetToString(packet));
        errorMessage_ = "Connecting...";
    } else {
        errorMessage_ = "Client ID is empty!";
    }
}

void Menu::handleTextInput()
{
    int key = GetCharPressed();

    while (key > 0) {
        if (key >= 32 && key <= 126) {
            clientId_ += static_cast<char>(key);
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !clientId_.empty()) {
        clientId_.pop_back();
    }
}

////////////////////////////////////////////// MAIN MENU /////////////////////////////////////////////////////////
void Menu::drawMainMenu()
{
    DrawText("Main Menu", 350, 100, 30, WHITE);

    for (auto &button : buttons_) {
        button.draw();
        DrawRectangleLines(button.rect.x, button.rect.y, button.rect.width, button.rect.height, RED);
    }

    Vector2 mousePos = GetMousePosition();
}

void Menu::Button::draw()
{
    DrawRectangleRec(rect, hovered ? DARKGRAY : GRAY);
    DrawText(text.c_str(), rect.x + 20, rect.y + 15, 20, WHITE);
}

void Menu::disconnectClient()
{
    sf::Packet packet;
    packet << DISCONNECT_PLAYER << clientId_;
    client_.send_message(packetToString(packet));
}

/////////////////////////////// CREATE ROOM ///////////////////////////////////////////
void Menu::drawCreateRoomScreen()
{
    DrawText("Create a Room", 300, 100, 30, WHITE);

    Color roomNameColor = isEditingRoomName_ ? GRAY : DARKGRAY;
    DrawRectangle(250, 240, 300, 30, roomNameColor);
    DrawText("Room Name:", 250, 200, 20, WHITE);
    DrawText(roomName_.c_str(), 255, 245, 20, WHITE);

    Color roomLevelColor = !isEditingRoomName_ ? GRAY : DARKGRAY;
    DrawRectangle(250, 340, 300, 30, roomLevelColor);
    DrawText("Level (1-8):", 250, 300, 20, WHITE);
    DrawText(std::to_string(roomLevel_).c_str(), 255, 345, 20, WHITE);

    DrawText("Press ENTER to create the room", 250, 400, 20, WHITE);

    if (!errorMessage_.empty()) {
        DrawText(errorMessage_.c_str(), 250, 500, 20, RED);
    }

    Rectangle backButton = {10, 550, 100, 40};
    DrawRectangleRec(backButton, GRAY);
    DrawText("Back", backButton.x + 20, backButton.y + 10, 20, WHITE);
}

void Menu::handleRoomCreationInput()
{
    Rectangle backButton = {10, 550, 100, 40};
    if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showCreateRoomScreen_ = false;
        roomName_.clear();
        roomLevel_ = 1;
        return;
    }

    if (IsKeyPressed(KEY_TAB)) {
        isEditingRoomName_ = !isEditingRoomName_;
    }

    if (isEditingRoomName_) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 126) {
                roomName_ += static_cast<char>(key);
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !roomName_.empty()) {
            roomName_.pop_back();
        }
    } else {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_Z)) {
            if (roomLevel_ < 8) {
                roomLevel_ += 1;
            } else {
                std::cout << "Room level is already at maximum." << std::endl;
            }
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            if (roomLevel_ > 1) {
                roomLevel_ -= 1;
            } else {
                std::cout << "Room level is already at minimum." << std::endl;
            }
        }
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (roomName_.empty() || roomLevel_ < 1 || roomLevel_ > 8) {
            errorMessage_ = "Invalid room name or level.";
            return;
        }
        sf::Packet packet;
        packet << CREATE_ROOM << clientId_ << roomName_ << roomLevel_;
        std::cout << "Client sending CREATE_ROOM action to server : " << clientId_ << " " << roomName_ << " et level: " << roomLevel_ << std::endl;
        client_.send_message(packetToString(packet));
        errorMessage_ = "Creating room...";
    }
}

/////////////////////////////////////////// SERACH ROOM ///////////////////////////////////////

void Menu::requestRooms()
{
    sf::Packet packet;
    packet << REQUEST_ROOMS;
    client_.send_message(packetToString(packet));
    errorMessage_ = "Requesting rooms...";
}

void Menu::drawSearchRoomScreen()
{
    DrawText("Available Rooms", 300, 50, 30, WHITE);
    Rectangle backButton = {10, 550, 100, 40};
    DrawRectangleRec(backButton, GRAY);
    DrawText("Back", backButton.x + 20, backButton.y + 10, 20, WHITE);

    int startY = 100 - scrollOffset_;
    if (startY < 0) {
        startY = 0;
    }

    for (size_t i = 0; i < rooms_.size(); ++i) {
        Room &room = rooms_[i];
        room.rect = {100, startY, 600, 90};
        DrawRectangleRec(room.rect, GRAY);
        DrawText(room.name.c_str(), room.rect.x + 10, room.rect.y + 10, 20, WHITE);
        DrawText(TextFormat("Level: %d", room.level), room.rect.x + 200, room.rect.y + 10, 20, WHITE);
        DrawText(TextFormat("Players: %d", room.players), room.rect.x + 400, room.rect.y + 10, 20, WHITE);
        Rectangle joinButton = {room.rect.x + 510, room.rect.y + 30, 80, 30};
        bool hovered = CheckCollisionPointRec(GetMousePosition(), joinButton);
        DrawRectangleRec(joinButton, hovered ? DARKGRAY : LIGHTGRAY);
        DrawText("Join", joinButton.x + 20, joinButton.y + 5, 20, WHITE);
        startY += 110;
    }
}

void Menu::handleSearchRoomInput()
{
    requestRooms();
    Rectangle backButton = {10, 550, 100, 40};
    if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showSearchRoomScreen_ = false;
        return;
    }

    int startY = 100 - scrollOffset_;
    for (size_t i = 0; i < rooms_.size(); ++i) {
        Room &room = rooms_[i];
        room.rect = {100, startY, 600, 90};
        Rectangle joinButton = {room.rect.x + 510, room.rect.y + 30, 80, 30};

        if (CheckCollisionPointRec(GetMousePosition(), joinButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sf::Packet packet;
            packet << JOIN_ROOM << clientId_ << room.name << skinPath_;
            client_.send_message(packetToString(packet));
            errorMessage_ = "Joining room...";
        }
        startY += 110;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_Z)) {
        scrollOffset_ = std::max(0, scrollOffset_ - 90);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        scrollOffset_ = std::min(static_cast<int>(rooms_.size() * 110 - 400), scrollOffset_ + 90);
    }
}

/////////////////////////////////////// OPTION //////////////////////////////////////////////////////////

void Menu::drawOptionsScreen()
{
    DrawText("Options", 350, 50, 30, WHITE);

    // Contrôle du volume
    DrawText("Volume", 250, 150, 20, WHITE);
    DrawRectangle(250, 200, 300, 30, GRAY);
    DrawRectangle(250, 200, volumeLevel_ * 3, 30, LIGHTGRAY);
    DrawText(mute_ ? "Muted" : TextFormat("%d%%", volumeLevel_), 570, 205, 20, WHITE);

    Rectangle muteButton = {250, 250, 150, 40};
    DrawRectangleRec(muteButton, GRAY);
    DrawText("Mute", muteButton.x + 20, muteButton.y + 10, 20, WHITE);

    // Informations client
    DrawText("Client Info", 250, 320, 20, WHITE);
    DrawText(TextFormat("Client ID: %s", clientId_.c_str()), 250, 350, 20, WHITE);
    DrawText(TextFormat("Host: %s", serverHost_.c_str()), 250, 380, 20, WHITE);
    DrawText(TextFormat("Port: %d", serverPort_), 250, 410, 20, WHITE);

    // Bouton Help
    Rectangle helpButton = {250, 470, 150, 40};
    DrawRectangleRec(helpButton, GRAY);
    DrawText("Help", helpButton.x + 20, helpButton.y + 10, 20, WHITE);

    // Bouton retour
    Rectangle backButton = {10, 550, 100, 40};
    DrawRectangleRec(backButton, GRAY);
    DrawText("Back", backButton.x + 20, backButton.y + 10, 20, WHITE);
}

void Menu::handleOptionsInput()
{
    Vector2 mousePos = GetMousePosition();

    // Gestion du slider de volume
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mousePos, {250, 200, 300, 30})) {
        volumeLevel_ = (mousePos.x - 250) / 3;
        volumeLevel_ = std::clamp(volumeLevel_, 0, 100);
        mute_ = false;
        SetSoundVolume(menuSound_, volumeLevel_ / 100.0f); // Adapter avec votre son
    }
    // Bouton mute
    Rectangle muteButton = {250, 250, 150, 40};
    if (CheckCollisionPointRec(mousePos, muteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mute_ = !mute_;
        SetSoundVolume(menuSound_, mute_ ? 0.0f : volumeLevel_ / 100.0f); // Adapter avec votre son
    }

    // Bouton retour
    Rectangle backButton = {10, 550, 100, 40};
    if (CheckCollisionPointRec(mousePos, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showOptionsScreen_ = false;
    }

    // Bouton Help (vide pour l'instant)
    Rectangle helpButton = {250, 470, 150, 40};
    if (CheckCollisionPointRec(mousePos, helpButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Placeholder pour le Help
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::drawSkinScreen()
{
    DrawText("Choose Your Skin", 300, 50, 30, WHITE);
    DrawText("Use Z and W to change skin and Enter to valid", 245, 100, 20, WHITE);

    try {
        Texture2D texture = textureManager_.getTexture(skins_[selectedSkinIndex_]);
        
        const int frameCount = 5;
        int frameWidth = texture.width / frameCount;
        int frameHeight = texture.height;

        int currentFrame = static_cast<int>(GetTime() * 10) % frameCount;

        Rectangle sourceRect = {
            static_cast<float>(currentFrame * frameWidth), 
            0,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
        };

        Rectangle destRect = {
            360,
            260,
            static_cast<float>(frameWidth * 3),
            static_cast<float>(frameHeight * 4)
        };

        DrawTexturePro(texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);

    } catch (const std::exception &e) {
        std::cerr << "[Menu::drawSkinScreen] Error: " << e.what() << std::endl;
        DrawText("Failed to load texture!", 360, 260, 20, RED);
    }

    Rectangle backButton = {10, 550, 100, 40};
    DrawRectangleRec(backButton, GRAY);
    DrawText("Back", backButton.x + 20, backButton.y + 10, 20, WHITE);
}



void Menu::handleSkinInput()
{
    Rectangle backButton = {10, 550, 100, 40};
    if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showSkinScreen_ = false;
        return;
    }

    if (IsKeyPressed(KEY_Z)) {
        std::cout << "KEY RIGHT" << std::endl;
        selectedSkinIndex_ += 1;
        if (selectedSkinIndex_ >= 4) {
            selectedSkinIndex_ = 0;
        }
    } else if (IsKeyPressed(KEY_W)) {
        std::cout << "KEY LEFT" << std::endl;
        selectedSkinIndex_ -= 1;
        if (selectedSkinIndex_ < 0) {
            selectedSkinIndex_ = 4;
        }
    }
    if (IsKeyPressed(KEY_ENTER)) {
        std::cout << "KEY ENTER with path = " << skins_[selectedSkinIndex_] << std::endl;
        if (!skins_.empty()) {
            skinPath_ = skins_[selectedSkinIndex_];
        }

        showSkinScreen_ = false;
    }
}
