#include "../../include/MenuState.h"
#include "../../include/GameController.h"
#include "../../include/CharacterSelectionState.h" // <--- AJOUT IMPORTANT
#include "../../include/OptionState.h"
// BattleState.h n'est plus nécessaire ici car on ne le lance plus directement
#include <iostream>

MenuState::MenuState(sf::Font& font, float width, float height) : font(font) {
    // 1. Chargement de l'image du MENU
    if (!backgroundTexture.loadFromFile("assets/images/menu_bg.png")) {        
        // Fallback ou erreur
    }
    backgroundSprite.setTexture(backgroundTexture);

    // 2. Adaptation à l'écran (Scaling)
    sf::Vector2u texSize = backgroundTexture.getSize();
    backgroundSprite.setScale(width / texSize.x, height / texSize.y);

    // 1. Titre
    titleText.setFont(font);
    titleText.setString("CODE FIGHTER");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    centerText(titleText, width / 2, height * 0.2f);

    // 2. Options
    std::string options[] = { "JOUER", "OPTIONS", "QUITTER" };
    for (int i = 0; i < 3; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(40);
        text.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White); // Le premier est sélectionné
        centerText(text, width / 2, height * 0.5f + (i * 100));
        menuOptions.push_back(text);
    }
}

void MenuState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}

void MenuState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOptionIndex = (selectedOptionIndex - 1 + menuOptions.size()) % menuOptions.size();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedOptionIndex = (selectedOptionIndex + 1) % menuOptions.size();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            
            // --- C'EST ICI QUE TOUT SE JOUE ---
            if (selectedOptionIndex == 0) {
                // JOUER -> On lance la SÉLECTION des personnages
                game.changeState(std::make_unique<CharacterSelectionState>(game));
            } 
            else if (selectedOptionIndex == 1) {
                // OPTIONS
                game.changeState(std::make_unique<OptionState>(game));
            } 
            else if (selectedOptionIndex == 2) {
                // QUITTER
                game.window.close();
            }
        }
    }
}

void MenuState::update(GameController& game) {
    // Animation simple de sélection
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        if (i == selectedOptionIndex) {
            menuOptions[i].setFillColor(sf::Color::Yellow);
            menuOptions[i].setScale(1.2f, 1.2f);
        } else {
            menuOptions[i].setFillColor(sf::Color::White);
            menuOptions[i].setScale(1.0f, 1.0f);
        }
    }
}

void MenuState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);

    window.draw(titleText);
    for (const auto& option : menuOptions) {
        window.draw(option);
    }
}