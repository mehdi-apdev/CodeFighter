#ifndef CHARACTERVIEW_H
#define CHARACTERVIEW_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../models/Character.h" // The Model

class CharacterView {
private:
    sf::Sprite sprite;
    // sf::Texture texture; // <-- Removed
    sf::Text nameText;
    // sf::Font& font; // <-- Removed
    Character& model; // Reference to the associated Model
    float barWidth = 350.f;
    float barHeight = 30.f;

    void drawHealthBar(sf::RenderWindow& window, float x, float y);

public:
    // Constructor: no longer takes font as parameter.
    CharacterView(Character& model, const std::string& texturePath, float xPos, float yPos, float scale);

    // Method to draw all graphic elements of the character
    void draw(sf::RenderWindow& window);
};

#endif // CHARACTERVIEW_H
