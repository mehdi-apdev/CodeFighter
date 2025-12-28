#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

// Forward declaration pour �viter les inclusions cycliques
class GameController;

class State {
public:
    virtual ~State() = default;

    // M�thodes virtuelles pures que chaque �tat doit impl�menter
    virtual void handleInput(GameController& game, sf::Event& event) = 0;
    virtual void update(GameController& game) = 0;
    virtual void render(GameController& game, sf::RenderWindow& window) = 0;
};

#endif
