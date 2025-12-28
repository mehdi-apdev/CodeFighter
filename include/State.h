#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

// Forward declaration pour éviter les inclusions cycliques
class GameController;

class State {
public:
    virtual ~State() = default;

    // Méthodes virtuelles pures que chaque état doit implémenter
    virtual void handleInput(GameController& game, sf::Event& event) = 0;
    virtual void update(GameController& game) = 0;
    virtual void render(GameController& game, sf::RenderWindow& window) = 0;
};

#endif
