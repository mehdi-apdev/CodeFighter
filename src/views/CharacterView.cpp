#include "CharacterView.h"
#include <iostream>

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
    nameText.setFont(font);
    nameText.setString(model.getName());
    nameText.setCharacterSize(24);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(xPos, yPos - 30.f); // Au-dessus de la barre de vie
}

// Méthode pour dessiner la barre de vie
void CharacterView::drawHealthBar(sf::RenderWindow& window, float x, float y) {
    int currentHealth = model.getHealth();
    int maxHealth = model.getMaxHealth();

    // Fond (gris/noir)
    sf::RectangleShape background(sf::Vector2f(barWidth, barHeight));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    window.draw(background);

    // Vie restante
    float healthRatio = (float)currentHealth / maxHealth;
    sf::RectangleShape foreground(sf::Vector2f(barWidth * healthRatio, barHeight));
    foreground.setPosition(x, y);

    // Couleur
    if (healthRatio > 0.5f) {
        foreground.setFillColor(sf::Color::Green);
    } else if (healthRatio > 0.2f) {
        foreground.setFillColor(sf::Color::Yellow);
    } else {
        foreground.setFillColor(sf::Color::Red);
    }

    window.draw(foreground);
}

// Méthode principale de dessin
void CharacterView::draw(sf::RenderWindow& window) {
    // Dessiner le nom
    window.draw(nameText);

    // Dessiner la barre de vie (la barre de vie est positionnée à l'endroit du sprite, ajustée)
    drawHealthBar(window, nameText.getPosition().x, nameText.getPosition().y + 30.f);

    // Dessiner le sprite
    window.draw(sprite);
}
