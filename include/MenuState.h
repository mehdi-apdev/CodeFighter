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
    sf::Font& font; // Référence vers la police du jeu
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    MenuState(sf::Font& font, float width, float height); // Constructeur

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;

    // Helper pour centrer
    void centerText(sf::Text& text, float x, float y);
};

#endif
