#include "../../include/controllers/GameController.h"
#include "../../include/states/MenuState.h"
#include "core/ResourceManager.h"
#include <iostream>
#include <algorithm>

// --- CONSTRUCTOR ---
GameController::GameController()
    // Initialize the window here to create the OpenGL context before any other SFML objects.
    : window(sf::VideoMode(1920, 1080), "CodeFighters", sf::Style::Default)
{
    window.setFramerateLimit(60);

    // Center the window on the screen
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        desktop.width / 2 - window.getSize().x / 2,
        desktop.height / 2 - window.getSize().y / 2
    ));

    // Initialize music (textures and fonts are now lazy-loaded)
    initResources();
    playMenuMusic();

    // Now that the window is ready, create the initial state.
    currentState = std::make_unique<MenuState>((float)window.getSize().x, (float)window.getSize().y);
}


// --- RESOURCE & MUSIC MANAGEMENT ---
void GameController::initResources() {
    // Only music is handled here. Fonts/textures are lazy-loaded by the ResourceManager.
    if (!MenuMusic.openFromFile("assets/music/menuCodeFighter.wav")) {
        std::cerr << "Error: Could not load menuCodeFighter.wav" << std::endl;
    }
    if (!BattleMusic.openFromFile("assets/music/CombatCodeFighter.wav")) {
        std::cerr << "Error: Could not load CombatCodeFighter.wav" << std::endl;
    }

    MenuMusic.setLoop(true);
    BattleMusic.setLoop(true);
    setMusicVolume(globalVolume);
}

void GameController::setMusicVolume(float vol) {
    globalVolume = std::max(0.f, std::min(100.f, vol));
    MenuMusic.setVolume(globalVolume);
    BattleMusic.setVolume(globalVolume);
}

void GameController::playMenuMusic() {
    if (BattleMusic.getStatus() == sf::Music::Playing) BattleMusic.stop();
    if (MenuMusic.getStatus() != sf::Music::Playing) MenuMusic.play();
}

void GameController::playBattleMusic() {
    if (MenuMusic.getStatus() == sf::Music::Playing) MenuMusic.stop();
    if (BattleMusic.getStatus() != sf::Music::Playing) BattleMusic.play();
}

void GameController::stopMusics() {
    MenuMusic.stop();
    BattleMusic.stop();
}


// --- STATE MANAGEMENT & GAME LOOP ---
void GameController::changeState(std::unique_ptr<State> newState) {
    if (newState) {
        currentState = std::move(newState);
    }
}

void GameController::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (currentState) {
                currentState->handleInput(*this, event);
            }
        }

        if (currentState) {
            currentState->update(*this);
        }

        window.clear();
        if (currentState) {
            currentState->render(*this, window);
        }
        window.display();
    }
}