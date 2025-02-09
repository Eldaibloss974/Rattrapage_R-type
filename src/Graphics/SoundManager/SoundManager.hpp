/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SoundManager.hpp
*/

#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <iostream>

#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <iostream>

class SoundManager {
    public:
        SoundManager() {
            InitAudioDevice();
            std::cout << "[SoundManager] Audio device initialized." << std::endl;
        }
    
        ~SoundManager() {
            for (auto &[key, sound] : soundCache_) {
                UnloadSound(sound);
                std::cout << "[SoundManager] Unloaded sound: " << key << std::endl;
            }
    
            if (musicInitialized_) {
                StopMusicStream(backgroundMusic_);
                UnloadMusicStream(backgroundMusic_);
                std::cout << "[SoundManager] Unloaded background music." << std::endl;
            }
    
            CloseAudioDevice();
            std::cout << "[SoundManager] Audio device closed." << std::endl;
        }
    
        void addSound(const std::string &filePath) {
            if (soundCache_.find(filePath) == soundCache_.end()) {
                Sound sound = LoadSound(filePath.c_str());
                soundCache_[filePath] = sound;
                std::cout << "[SoundManager] Loaded sound: " << filePath << std::endl;
            } else {
                std::cout << "[SoundManager] Sound already in cache: " << filePath << std::endl;
            }
        }
    
        void playSound(const std::string &filePath, float volume = 1.0f) {
            if (soundCache_.find(filePath) == soundCache_.end()) {
                addSound(filePath);
            }
    
            Sound &sound = soundCache_[filePath];
            SetSoundVolume(sound, volume);
            PlaySound(sound);
            std::cout << "[SoundManager] Playing sound: " << filePath << " at volume: " << volume << std::endl;
        }
    
        void loadBackgroundMusic(const std::string &filePath) {
            if (!musicInitialized_) {
                backgroundMusic_ = LoadMusicStream(filePath.c_str());
                musicInitialized_ = true;
                std::cout << "[SoundManager] Loaded background music: " << filePath << std::endl;
            } else {
                std::cerr << "[SoundManager] Background music already loaded." << std::endl;
            }
        }
    
        void playBackgroundMusic(float volume = 1.0f) {
            if (musicInitialized_) {
                PlayMusicStream(backgroundMusic_);
                SetMusicVolume(backgroundMusic_, volume);
                std::cout << "[SoundManager] Playing background music at volume: " << volume << std::endl;
            } else {
                std::cerr << "[SoundManager] No background music loaded." << std::endl;
            }
        }
    
        void updateBackgroundMusic() {
            if (musicInitialized_) {
                UpdateMusicStream(backgroundMusic_);
            }
        }
    
        // void togglePauseBackgroundMusic() {

        // }
    
        void stopBackgroundMusic() {
            if (musicInitialized_) {
                StopMusicStream(backgroundMusic_);
                std::cout << "[SoundManager] Background music stopped." << std::endl;
            }
        }
    
    private:
        std::unordered_map<std::string, Sound> soundCache_;
        Music backgroundMusic_;
        bool musicInitialized_ = false;
};
