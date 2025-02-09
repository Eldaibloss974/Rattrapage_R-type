/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** TextureManager.hpp
*/

#pragma once

#include "raylib.h"
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>

class TextureManager {
    private:
        std::unordered_map<std::string, Texture2D> textureCache;
    
    public:
        TextureManager() {
            textureCache = std::unordered_map<std::string, Texture2D>();
            // std::cout << "[TextureManager] Initialized texture cache. Size: " << textureCache.size() << std::endl;
        }

        ~TextureManager() {
            if (!IsWindowReady()) {
                // std::cerr << "[TextureManager] Window is not ready. Skipping texture unloading." << std::endl;
                return;
            }
            for (auto& [path, texture] : textureCache) {
                if (texture.id != 0) {
                    // std::cout << "[TextureManager] Unloading texture: " << path << ", ID: " << texture.id << std::endl;
                UnloadTexture(texture);
                }
            }
            textureCache.clear();
            // std::cout << "[TextureManager] Texture cache cleared." << std::endl;
        }

        size_t getCacheSize() const {
            return textureCache.size();
        }

    
        Texture2D getTexture(const std::string& path) {
            // std::cout << "[TextureManager] Requesting texture for path: " << path << std::endl;
            if (textureCache.empty()) {
                // std::cout << "[DEBUG] textureCache is empty." << std::endl;
            } else {
                // std::cout << "[DEBUG] textureCache has " << textureCache.size() << " elements." << std::endl;
            }


            std::ifstream file(path);
            if (!file.good()) {
                // std::cerr << "[TextureManager] Error: File does not exist at path: " << path << std::endl;
                throw std::runtime_error("File not found: " + path);
            }
            if (textureCache.find(path) != textureCache.end()) {
                // std::cout << "[TextureManager] Texture found in cache for path: " << path << std::endl;
                return textureCache[path];
            }
    
            // std::cout << "[TextureManager] Texture not in cache, loading from file: " << path << std::endl;
            Texture2D texture = LoadTexture(path.c_str());
            if (texture.id == 0) {
                // std::cerr << "[TextureManager] Failed to load texture from path: " << path << std::endl;
                throw std::runtime_error("Failed to load texture: " + path);
            }
    
            // std::cout << "[TextureManager] Texture loaded successfully. Caching texture for path: " << path << std::endl;
            textureCache[path] = texture;
            return texture;
        }

};
