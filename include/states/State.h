#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

// Forward declaration to avoid circular inclusions
class GameController;

class State {
public:
    virtual ~State() = default;

    // Pure virtual methods that each state must implement
    virtual void handleInput(GameController& game, sf::Event& event) = 0;
    virtual void update(GameController& game) = 0;
    virtual void render(GameController& game, sf::RenderWindow& window) = 0;
};

#endif
