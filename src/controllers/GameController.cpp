#include "../../include/GameController.h"
#include "../../include/MenuState.h" // État initial
#include <iostream>

GameController::GameController()
    : window(sf::VideoMode(1920, 1080), "CodeFighter - MVC & State Pattern") {

    window.setFramerateLimit(60);
    initResources();

    // Démarrage directement sur le MENU
    currentState = std::make_unique<MenuState>(font, 1920.f, 1080.f);
}

void GameController::initResources() {
    if (!font.loadFromFile("C:/Users/Pawla/OneDrive/Bureau/Bac3/jeux/codeF/playgroundcodeblocks/assets/fonts/ARIAL.TTF")) std::cerr << "Err Font" << std::endl;

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
