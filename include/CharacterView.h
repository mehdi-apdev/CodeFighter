#ifndef CHARACTERVIEW_H
#define CHARACTERVIEW_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Character.h" // The Model

class CharacterView {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text nameText;
    sf::Font& font;
    Character& model; // Reference to the associated Model
    
    // NEW: Store the base position to retrieve it later
    sf::Vector2f basePosition;

    float barWidth = 350.f;
    float barHeight = 30.f;

    void drawHealthBar(sf::RenderWindow& window, float x, float y);

public:
    // Constructor: takes the Model reference, image path, font and positions.
    CharacterView(Character& model, const std::string& texturePath, sf::Font& font, float xPos, float yPos, float scale);

    // Method to draw all graphic elements of the character
    void draw(sf::RenderWindow& window);

    // NEW: Getters required for BattleState
    Character& getCharacter() const;
    sf::Vector2f getPosition() const;
};

#endif // CHARACTERVIEW_H