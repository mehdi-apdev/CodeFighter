#include "../../include/OptionState.h"
#include "../../include/GameController.h"
#include "../../include/MenuState.h"
#include <string>

OptionState::OptionState(GameController& game) : font(game.font) {
    // Fond d'écran (réutilisation du menu ou autre)
    if (backgroundTexture.loadFromFile("assets/images/option_bg.png")) {
        backgroundSprite.setTexture(backgroundTexture);
        sf::Vector2u texSize = backgroundTexture.getSize();
        backgroundSprite.setScale(1920.f / texSize.x, 1080.f / texSize.y);
    }

    // Titre
    titleText.setFont(font);
    titleText.setString("OPTIONS");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    centerText(titleText, 1920 / 2, 200);

    // Label Volume
    volumeLabel.setFont(font);
    volumeLabel.setString("Volume Musique :");
    volumeLabel.setCharacterSize(50);
    centerText(volumeLabel, 1920 / 2, 450);

    // Barre du Slider (Le rail)
    sliderBar.setSize(sf::Vector2f(600, 20));
    sliderBar.setFillColor(sf::Color(100, 100, 100));
    sliderBar.setOrigin(300, 10);
    sliderBar.setPosition(1920 / 2, 550);

    // Poignée du Slider (Le curseur)
    sliderHandle.setSize(sf::Vector2f(30, 60));
    sliderHandle.setFillColor(sf::Color::Red);
    sliderHandle.setOrigin(15, 30);
    
    // Calcul de la position initiale du curseur basé sur le volume actuel
    float ratio = game.globalVolume / 100.0f;
    float handleX = (1920 / 2 - 300) + (ratio * 600); 
    sliderHandle.setPosition(handleX, 550);

    // Valeur textuelle (ex: "50%")
    valueText.setFont(font);
    valueText.setString(std::to_string((int)game.globalVolume) + "%");
    centerText(valueText, 1920 / 2, 620);

    // Bouton Retour
    backText.setFont(font);
    backText.setString("RETOUR (Echap)");
    backText.setCharacterSize(40);
    backText.setFillColor(sf::Color::White);
    centerText(backText, 1920 / 2, 850);
}

void OptionState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}

void OptionState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        // Retour au menu
        game.changeState(std::make_unique<MenuState>(game.font, 1920.f, 1080.f));
    }

    // Gestion de la souris pour le slider
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (sliderHandle.getGlobalBounds().contains(mousePos) || sliderBar.getGlobalBounds().contains(mousePos)) {
            isDragging = true;
        }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }
}

void OptionState::update(GameController& game) {
    if (isDragging) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(game.window);
        sf::Vector2f mousePos = game.window.mapPixelToCoords(pixelPos);

        // Limites du slider (Gauche et Droite)
        float leftLimit = 1920 / 2 - 300;
        float rightLimit = 1920 / 2 + 300;

        // Clamper la souris entre les limites
        float newX = std::max(leftLimit, std::min(mousePos.x, rightLimit));
        sliderHandle.setPosition(newX, 550);

        // Calcul du pourcentage (0.0 à 1.0)
        float ratio = (newX - leftLimit) / 600.0f;
        float newVolume = ratio * 100.0f;

        // Mise à jour du jeu
        game.setMusicVolume(newVolume);
        valueText.setString(std::to_string((int)newVolume) + "%");
        centerText(valueText, 1920 / 2, 620); // Recentrer car le texte change
    }
}

void OptionState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(volumeLabel);
    window.draw(sliderBar);
    window.draw(sliderHandle);
    window.draw(valueText);
    window.draw(backText);
}