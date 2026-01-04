#include "../../include/views/CharacterView.h"
#include "core/ResourceManager.h"
#include <iostream>

// Constructor
CharacterView::CharacterView(Character& model, const std::string& texturePath, float xPos, float yPos, float scale)
    : model(model) {

    // 1. Get the texture from the ressource manager
    sprite.setTexture(ResourceManager::getInstance().getTexture(texturePath));
    sprite.setPosition(xPos, yPos + 40.f); // Décaler l'image sous la barre de vie
    sprite.setScale(scale, scale);

    // 2. Initialize the name's text with the font from the ressource manager
    nameText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    nameText.setString(model.getName());
    nameText.setCharacterSize(24);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(xPos, yPos - 30.f); // Above the health bar
}

// Method used to draw the health bar for a Character
void CharacterView::drawHealthBar(sf::RenderWindow& window, float x, float y) {
    int currentHealth = model.getHealth();
    int maxHealth = model.getMaxHealth();

    // Background (Grey, Black)
    sf::RectangleShape background(sf::Vector2f(barWidth, barHeight));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    window.draw(background);

    // Remainig health
    float healthRatio = (float)currentHealth / maxHealth;
    sf::RectangleShape foreground(sf::Vector2f(barWidth * healthRatio, barHeight));
    foreground.setPosition(x, y);

    // Colour
    if (healthRatio > 0.5f) {
        foreground.setFillColor(sf::Color::Green);
    } else if (healthRatio > 0.2f) {
        foreground.setFillColor(sf::Color::Yellow);
    } else {
        foreground.setFillColor(sf::Color::Red);
    }

    window.draw(foreground);
}

// Method used to draw the CharacterView in the window
void CharacterView::draw(sf::RenderWindow& window) {
    // Draw the name
    window.draw(nameText);

    // Draw the health bar
    drawHealthBar(window, nameText.getPosition().x, nameText.getPosition().y + 30.f);

    // Draw the sprite
    window.draw(sprite);
}
