#include "../../include/CharacterView.h"
#include <iostream>

<<<<<<< HEAD
// Constructeur
CharacterView::CharacterView(Character& model, const std::string& texturePath, sf::Font& font, float xPos, float yPos, float scale)
    : model(model), font(font) {

    // 1. Charger la Texture
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur: Impossible de charger la texture " << texturePath << " pour " << model.getName() << "!" << std::endl;
        // Gérer l'erreur ou utiliser une texture par défaut
    }
    sprite.setTexture(texture);
    sprite.setPosition(xPos, yPos + 40.f); // Décaler l'image sous la barre de vie
    sprite.setScale(scale, scale);

    // 2. Initialiser le Texte du Nom
=======
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
>>>>>>> aa70bc9632c461277d4d99e227cfd2860db63c10
    nameText.setFont(font);
    nameText.setString(model.getName());
    nameText.setCharacterSize(24);
    nameText.setFillColor(sf::Color::White);
<<<<<<< HEAD
    nameText.setPosition(xPos, yPos - 30.f); // Au-dessus de la barre de vie
}

// Méthode pour dessiner la barre de vie
=======
    nameText.setPosition(xPos, yPos - 30.f); // Above health bar
}

// Method to draw health bar
>>>>>>> aa70bc9632c461277d4d99e227cfd2860db63c10
void CharacterView::drawHealthBar(sf::RenderWindow& window, float x, float y) {
    int currentHealth = model.getHealth();
    int maxHealth = model.getMaxHealth();

<<<<<<< HEAD
    // Fond (gris/noir)
=======
    // Background (gray/black)
>>>>>>> aa70bc9632c461277d4d99e227cfd2860db63c10
    sf::RectangleShape background(sf::Vector2f(barWidth, barHeight));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    window.draw(background);

<<<<<<< HEAD
    // Vie restante
=======
    // Remaining health
>>>>>>> aa70bc9632c461277d4d99e227cfd2860db63c10
    float healthRatio = (float)currentHealth / maxHealth;
    sf::RectangleShape foreground(sf::Vector2f(barWidth * healthRatio, barHeight));
    foreground.setPosition(x, y);

<<<<<<< HEAD
    // Couleur
=======
    // Color
>>>>>>> aa70bc9632c461277d4d99e227cfd2860db63c10
    if (healthRatio > 0.5f) {
        foreground.setFillColor(sf::Color::Green);
    } else if (healthRatio > 0.2f) {
        foreground.setFillColor(sf::Color::Yellow);
    } else {
        foreground.setFillColor(sf::Color::Red);
    }

    window.draw(foreground);
}

<<<<<<< HEAD
// Méthode principale de dessin
void CharacterView::draw(sf::RenderWindow& window) {
    // Dessiner le nom
    window.draw(nameText);

    // Dessiner la barre de vie (la barre de vie est positionnée à l'endroit du sprite, ajustée)
    drawHealthBar(window, nameText.getPosition().x, nameText.getPosition().y + 30.f);

    // Dessiner le sprite
    window.draw(sprite);
}
=======
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
>>>>>>> aa70bc9632c461277d4d99e227cfd2860db63c10
