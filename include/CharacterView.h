#ifndef CHARACTERVIEW_H
#define CHARACTERVIEW_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Character.h" // Le Model

class CharacterView {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text nameText;
    sf::Font& font;
    Character& model; // Référence au Model associé
    float barWidth = 350.f;
    float barHeight = 30.f;

    void drawHealthBar(sf::RenderWindow& window, float x, float y);

public:
    // Constructeur : prend la référence du Model, le chemin de l'image, la police et les positions.
    CharacterView(Character& model, const std::string& texturePath, sf::Font& font, float xPos, float yPos, float scale);

    // Méthode pour dessiner tous les éléments graphiques du personnage
    void draw(sf::RenderWindow& window);
};

#endif // CHARACTERVIEW_H
