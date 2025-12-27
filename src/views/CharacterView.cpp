#include "../../include/CharacterView.h"
#include <iostream>

// Constructor
CharacterView::CharacterView(Character& model, const std::string& texturePath, sf::Font& font, float xPos, float yPos, float scale)
    : model(model), font(font) {

    basePosition = sf::Vector2f(xPos, yPos);

    // 1. Load Texture
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Unable to load texture " << texturePath << " for " << model.getName() << "!" << std::endl;
        // Handle error or use default texture
    }
    sprite.setTexture(texture);
    sprite.setPosition(xPos, yPos + 40.f); // Offset image below health bar
    sprite.setScale(scale, scale);

    // 2. Initialize Name Text
    nameText.setFont(font);
    nameText.setString(model.getName());
    nameText.setCharacterSize(24);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(xPos, yPos - 30.f); // Above health bar
}

// Method to draw health bar
void CharacterView::drawHealthBar(sf::RenderWindow& window, float x, float y) {
    int currentHealth = model.getHealth();
    int maxHealth = model.getMaxHealth();

    // Background (gray/black)
    sf::RectangleShape background(sf::Vector2f(barWidth, barHeight));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    window.draw(background);

    // Remaining health
    float healthRatio = (float)currentHealth / maxHealth;
    sf::RectangleShape foreground(sf::Vector2f(barWidth * healthRatio, barHeight));
    foreground.setPosition(x, y);

    // Color
    if (healthRatio > 0.5f) {
        foreground.setFillColor(sf::Color::Green);
    } else if (healthRatio > 0.2f) {
        foreground.setFillColor(sf::Color::Yellow);
    } else {
        foreground.setFillColor(sf::Color::Red);
    }

    window.draw(foreground);
}

// Main draw method
void CharacterView::draw(sf::RenderWindow& window) {
    // Draw name
    window.draw(nameText);

    // Draw health bar (positioned at sprite location, adjusted)
    drawHealthBar(window, nameText.getPosition().x, nameText.getPosition().y + 30.f);

    // Draw sprite
    window.draw(sprite);
}

Character& CharacterView::getCharacter() const {
    return model;
}

sf::Vector2f CharacterView::getPosition() const {
    return basePosition;
}