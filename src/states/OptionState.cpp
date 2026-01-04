#include "../../include/states/OptionState.h"
#include "../../include/controllers/GameController.h"
#include "../../include/states/MenuState.h"
#include "core/ResourceManager.h"
#include <string>

OptionState::OptionState(GameController& game) {
    // Background (reusing menu background or other)
    backgroundSprite.setTexture(ResourceManager::getInstance().getTexture("assets/images/option_bg.png"));
    sf::Vector2u texSize = backgroundSprite.getTexture()->getSize();
    backgroundSprite.setScale(1920.f / texSize.x, 1080.f / texSize.y);

    // Title
    titleText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    titleText.setString("OPTIONS");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    centerText(titleText, 1920 / 2, 200);

    // Volume Label
    volumeLabel.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    volumeLabel.setString("Music Volume:");
    volumeLabel.setCharacterSize(50);
    centerText(volumeLabel, 1920 / 2, 450);

    // Slider Bar (The rail)
    sliderBar.setSize(sf::Vector2f(600, 20));
    sliderBar.setFillColor(sf::Color(100, 100, 100));
    sliderBar.setOrigin(300, 10);
    sliderBar.setPosition(1920 / 2, 550);

    // Slider Handle (The cursor)
    sliderHandle.setSize(sf::Vector2f(30, 60));
    sliderHandle.setFillColor(sf::Color::Red);
    sliderHandle.setOrigin(15, 30);
    
    // Calculate initial handle position based on current volume
    float ratio = game.globalVolume / 100.0f;
    float handleX = (1920 / 2 - 300) + (ratio * 600); 
    sliderHandle.setPosition(handleX, 550);

    // Text value (e.g., "50%")
    valueText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    valueText.setString(std::to_string((int)game.globalVolume) + "%");
    centerText(valueText, 1920 / 2, 620);

    // Back Button
    backText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    backText.setString("BACK (Esc)");
    backText.setCharacterSize(40);
    backText.setFillColor(sf::Color::White);
    centerText(backText, 1920 / 2, 850);
}

//Method used to center a specific text in the window
void OptionState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}

//Method used to handle the user's inputs
void OptionState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        // Back to menu
        game.changeState(std::make_unique<MenuState>(1920.f, 1080.f));
    }

    // Mouse handling for the slider
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

//Method used to update the graphical elements in the window
void OptionState::update(GameController& game) {
    if (isDragging) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(game.window);
        sf::Vector2f mousePos = game.window.mapPixelToCoords(pixelPos);

        // Slider limits (Left and Right)
        float leftLimit = 1920 / 2 - 300;
        float rightLimit = 1920 / 2 + 300;

        // Clamp mouse position between limits
        float newX = std::max(leftLimit, std::min(mousePos.x, rightLimit));
        sliderHandle.setPosition(newX, 550);

        // Calculate percentage (0.0 to 1.0)
        float ratio = (newX - leftLimit) / 600.0f;
        float newVolume = ratio * 100.0f;

        // Update game
        game.setMusicVolume(newVolume);
        valueText.setString(std::to_string((int)newVolume) + "%");
        centerText(valueText, 1920 / 2, 620); // Recenter because the text changes
    }
}

//Method used to draw the graphical elements in the window
void OptionState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(volumeLabel);
    window.draw(sliderBar);
    window.draw(sliderHandle);
    window.draw(valueText);
    window.draw(backText);
}