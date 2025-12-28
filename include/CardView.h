#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <SFML/Graphics.hpp>
#include "IAbility.h"

class CardView {
private:
    IAbility* ability; // Pointeur vers le Mod�le (Command)

    sf::RectangleShape cardShape;
    sf::Text nameText;
    sf::Text costText;
    sf::Text descText;

    bool isSelected = false;

public:
    CardView(IAbility* ability, sf::Font& font, float x, float y);

    // Dessin
    void draw(sf::RenderWindow& window);

    // Interaction (Controller -> View)
    bool isClicked(sf::Vector2i mousePos);
    void setPosition(float x, float y);
    void highlight(bool active); // Change la couleur si s�lectionn�e

    // Getter pour le Controller
    IAbility* getAbility() const { return ability; }
};

#endif
