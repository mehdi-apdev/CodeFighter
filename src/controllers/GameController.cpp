#include "../../include/GameController.h"
#include "../../include/MenuState.h" // État initial
#include <iostream>

GameController::GameController()
    : window(sf::VideoMode(1920, 1080), "CodeFighter - MVC & State Pattern") {

    window.setFramerateLimit(60);
    initResources();


    playMenuMusic();

    // Démarrage directement sur le MENU
    currentState = std::make_unique<MenuState>(font, 1920.f, 1080.f);
}

void GameController::initResources() {
    if (!font.loadFromFile("assets/fonts/ARIAL.TTF")) std::cerr << "Err Font" << std::endl;
    if (!MenuMusic.openFromFile("assets/music/menuCodeFighter.wav")) std::cerr << "Err Menu Music" << std::endl;
    if (!BattleMusic.openFromFile("assets/music/CombatCodeFighter.wav")) std::cerr << "Err Battle Music" << std::endl;

    MenuMusic.setLoop(true);
    BattleMusic.setLoop(true);
    

    // Initialisation du volume
    setMusicVolume(globalVolume);
}

// Implémentation de la fonction de volume
void GameController::setMusicVolume(float vol) {
    globalVolume = vol;
    // Bornage entre 0 et 100
    if (globalVolume < 0) globalVolume = 0;
    if (globalVolume > 100) globalVolume = 100;

    MenuMusic.setVolume(globalVolume);
    BattleMusic.setVolume(globalVolume);
}

void GameController::playMenuMusic() {
    // Si la musique de combat joue, on l'arrête
    if (BattleMusic.getStatus() == sf::Music::Playing) BattleMusic.stop();
    

    // Si la musique de menu ne joue pas, on la lance
    if (MenuMusic.getStatus() != sf::Music::Playing) MenuMusic.play();
}

void GameController::playBattleMusic() {
    // Si la musique de menu joue, on l'arrête
    if (MenuMusic.getStatus() == sf::Music::Playing) MenuMusic.stop();
    

    // Si la musique de combat ne joue pas, on la lance
    if (BattleMusic.getStatus() != sf::Music::Playing) BattleMusic.play();
}

void GameController::stopMusics() {
    MenuMusic.stop();
    BattleMusic.stop();
}

void GameController::changeState(std::unique_ptr<State> newState) {
    // Changement dynamique d'état
    currentState = std::move(newState);
}

void GameController::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();


            // Délégation de l'input à l'état actif (Menu ou Jeu)
            if (currentState) currentState->handleInput(*this, event);
        }

        // Délégation de la logique
        if (currentState) currentState->update(*this);

        // Rendu
        window.clear();

        if (currentState) currentState->render(*this, window); // Dessin spécifique
        window.display();
    }
}
