#!/bin/bash

# Vérifier les permissions root
check_root() {
    if [ "$(id -u)" -ne 0 ]; then
        echo "Ce script doit être exécuté en tant que root."
        exit 1
    fi
}

# Mettre à jour le système et installer les dépendances de base
install_dependencies() {
    echo "Mise à jour du système et installation des dépendances de base..."
    apt-get update
    apt-get install -y \
        build-essential \
        cmake \
        git \
        wget \
        libasound2-dev \
        libx11-dev \
        libxrandr-dev \
        libxi-dev \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libxcursor-dev \
        libxinerama-dev \
        libwayland-dev \
        libxkbcommon-dev \
        libboost-system-dev \
        libboost-thread-dev \
        libsfml-dev \
        python3 \
        python3-pip
}

# Installation de Raylib
install_raylib() {
    local RAYLIB_VERSION="4.0.0"
    local RAYLIB_HEADER_FILE="/usr/local/include/raylib.h"

    if [ -f "$RAYLIB_HEADER_FILE" ]; then
        echo "✅ Raylib $RAYLIB_VERSION est déjà installé."
    else
        echo "🚀 Installation de Raylib $RAYLIB_VERSION..."
        git clone https://github.com/raysan5/raylib.git /tmp/raylib
        cd /tmp/raylib
        mkdir build && cd build
        cmake .. && make && make install
        cd / && rm -rf /tmp/raylib
        echo "✅ Raylib $RAYLIB_VERSION installé avec succès."
    fi
}

install_boost() {
    local BOOST_VERSION="1.87.0"
    local BOOST_DIR="/usr/local"

    if [ -d "$BOOST_DIR/include/boost" ]; then
        echo "✅ Boost $BOOST_VERSION est déjà installé."
    else
        echo "🚀 Installation de Boost $BOOST_VERSION..."
        mkdir -p /tmp/boost && cd /tmp/boost
        wget -q https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_1_87_0.tar.gz
        tar xzf boost_1_87_0.tar.gz && cd boost_1_87_0
        ./bootstrap.sh --prefix=$BOOST_DIR --with-libraries=system,thread
        ./b2 install --prefix=$BOOST_DIR
        cd / && rm -rf /tmp/boost
        echo "✅ Boost $BOOST_VERSION installé avec succès."
    fi
}

# Nettoyage des fichiers temporaires
clean_up() {
    echo "Nettoyage des fichiers temporaires..."
    apt-get clean
}

# Exécution des fonctions
check_root
install_dependencies
install_raylib
install_boost
clean_up

echo "✅ Toutes les dépendances ont été installées avec succès."