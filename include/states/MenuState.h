#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

class MenuState : public State {
private:
    sf::Text titleText;
    std::vector<sf::Text> menuOptions;
    int selectedOptionIndex = 0;
    // sf::Font& font; // <-- Removed
    // sf::Texture backgroundTexture; // <-- Removed
    sf::Sprite backgroundSprite;

public:
    MenuState(float width, float height); // Simplified constructor

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;

    // Helper to center
    void centerText(sf::Text& text, float x, float y);
};

#endif