#ifndef OPTIONSTATE_H
#define OPTIONSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>

class OptionState : public State {
private:
    sf::Text titleText;
    sf::Text volumeLabel;
    sf::Text valueText;
    sf::Text backText;

    // Slider UI
    sf::RectangleShape sliderBar;
    sf::RectangleShape sliderHandle;
    bool isDragging = false;

    sf::Font& font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    OptionState(GameController& game);

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;

    void centerText(sf::Text& text, float x, float y);
};

#endif