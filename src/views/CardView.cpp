#include "../../include/CardView.h"

CardView::CardView(IAbility* ability, sf::Font& font, float x, float y)
    : ability(ability) {

    // 1. Fond de la carte
    cardShape.setSize(sf::Vector2f(120.f, 160.f));
    cardShape.setPosition(x, y);
    cardShape.setFillColor(sf::Color(40, 40, 60)); // Bleu fonc�
    cardShape.setOutlineThickness(2.f);
    cardShape.setOutlineColor(sf::Color::White);

    // 2. Textes (Nom, Co�t)
    nameText.setFont(font);
    nameText.setString(ability->getName());
    nameText.setCharacterSize(14);
    nameText.setPosition(x + 5, y + 5);

    costText.setFont(font);
    costText.setString(std::to_string(ability->getCost()));
    costText.setCharacterSize(20);
    costText.setFillColor(sf::Color::Cyan);
    costText.setPosition(x + 95, y + 5);
}

void CardView::draw(sf::RenderWindow& window) {
    window.draw(cardShape);
    window.draw(nameText);
    window.draw(costText);
}

void CardView::highlight(bool active) {
    if (active) cardShape.setOutlineColor(sf::Color::Green);
    else cardShape.setOutlineColor(sf::Color::White);
}

bool CardView::isClicked(sf::Vector2i mousePos) {
    return cardShape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}
