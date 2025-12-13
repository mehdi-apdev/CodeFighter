#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "State.h"

class GameController {
public:
    sf::RenderWindow window;
    sf::Font font;

    // LE COEUR DU PATTERN STATE
    std::unique_ptr<State> currentState;

    GameController();
    ~GameController() = default;

    void run();
    void changeState(std::unique_ptr<State> newState); // Pour passer de Menu à Battle

    // Ressources partagées
    void initResources();
};

#endif
