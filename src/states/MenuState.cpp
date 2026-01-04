#include "../../include/states/MenuState.h"
#include "../../include/controllers/GameController.h"
#include "../../include/states/CharacterSelectionState.h"
#include "../../include/states/OptionState.h"
#include "core/ResourceManager.h"
#include <iostream>

MenuState::MenuState(float width, float height) {
    // Get the global font by its full path
    sf::Font& font = ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF");
    
    // 1. Load MENU image via ResourceManager using its full path
    backgroundSprite.setTexture(ResourceManager::getInstance().getTexture("assets/images/menu_bg.png"));

    // 2. Screen adaptation (Scaling)
    sf::Vector2u texSize = backgroundSprite.getTexture()->getSize();
    backgroundSprite.setScale(width / texSize.x, height / texSize.y);

    // 1. Title
    titleText.setFont(font);
    titleText.setString("CODE FIGHTER");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    centerText(titleText, width / 2, height * 0.2f);

    // 2. Options
    std::string options[] = { "PLAY", "OPTIONS", "QUIT" };
    for (int i = 0; i < 3; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(40);
        text.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White); // The first one is selected
        centerText(text, width / 2, height * 0.5f + (i * 100));
        menuOptions.push_back(text);
    }
}

//Method used to center a specific text in the window
void MenuState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}

//Method used to manage the user's inputs
void MenuState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOptionIndex = (selectedOptionIndex - 1 + menuOptions.size()) % menuOptions.size();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedOptionIndex = (selectedOptionIndex + 1) % menuOptions.size();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            
            // --- THIS IS WHERE IT ALL HAPPENS ---
            if (selectedOptionIndex == 0) {
                // PLAY -> Launch character SELECTION
                game.changeState(std::make_unique<CharacterSelectionState>(game));
            } 
            else if (selectedOptionIndex == 1) {
                // OPTIONS
                game.changeState(std::make_unique<OptionState>(game));
            } 
            else if (selectedOptionIndex == 2) {
                // QUIT
                game.window.close();
            }
        }
    }
}

//Method used to update all the graphical elements in the window
void MenuState::update(GameController& game) {
    // Simple selection animation
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

//Method used to draw the graphical elements in the windows
void MenuState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);

    window.draw(titleText);
    for (const auto& option : menuOptions) {
        window.draw(option);
    }
}
