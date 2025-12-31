#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <SFML/Graphics.hpp>
#include "../abilities/IAbility.h"

class CardView {
private:
    IAbility* ability; // Pointer to the Model (Command)

    sf::RectangleShape cardShape;
    sf::Text nameText;
    sf::Text costText;
    sf::Text descText;

    bool isSelected = false;

public:
    // Constructor no longer takes font
    CardView(IAbility* ability, float x, float y);

    // Drawing
    void draw(sf::RenderWindow& window);

    // Interaction (Controller -> View)
    bool isClicked(sf::Vector2i mousePos);
    void setPosition(float x, float y);
    void highlight(bool active); // Changes color if selected

    // Getter for the Controller
    IAbility* getAbility() const { return ability; }
};

#endif
