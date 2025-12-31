#include "states/CharacterSelectionState.h"
#include "controllers/GameController.h"
#include "states/BattleState.h"
#include "core/ResourceManager.h"
#include <iostream>


// --- CONSTRUCTOR ---
CharacterSelectionState::CharacterSelectionState(GameController& game) 
{
    initRoster();
    // The loadAssets call is removed.
    initUI((float)game.window.getSize().x, (float)game.window.getSize().y);
}

// --- PRIVATE METHODS ---
void CharacterSelectionState::initRoster() {
    availableCharacters.emplace_back(1, "Pyra (Core)", 100, "The main engine.");
    availableCharacters.emplace_back(2, "Pyra (Venv)", 80, "The virtual environment.");
    availableCharacters.emplace_back(3, "Pyra (GIL)", 120, "The Global Interpreter Lock.");
    availableCharacters.emplace_back(4, "Java (JVM)", 90, "The virtual machine.");
    availableCharacters.emplace_back(5, "Java (GC)", 100, "The Garbage Collector.");
    availableCharacters.emplace_back(6, "Java (JIT)", 70, "The Just-In-Time compiler.");
}

// --- loadAssets method is removed ---

// --- UI INITIALIZATION ---
void CharacterSelectionState::initUI(float width, float height) {
    // Setup background from ResourceManager
    backgroundSprite.setTexture(ResourceManager::getInstance().getTexture("assets/images/Perso_Menu.png"));
    sf::Vector2u texSize = backgroundSprite.getTexture()->getSize();
    backgroundSprite.setScale(width / texSize.x, height / texSize.y);
    backgroundSprite.setColor(sf::Color(150, 150, 150)); 

    // 1. Title
    titleText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    titleText.setString("CHAMPION SELECTION");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);
    centerText(titleText, width / 2, 50);

    // 2. Instructions
    instructionText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    instructionText.setString("PLAYER 1: Choose 3 champions");
    instructionText.setCharacterSize(36);
    instructionText.setFillColor(sf::Color::Cyan);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(2.f);
    centerText(instructionText, width / 2, 120);

    // 3. Feedback (The confirmation text at the bottom)
    feedbackText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
    feedbackText.setString(""); // Empty at the start
    feedbackText.setCharacterSize(30);
    feedbackText.setFillColor(sf::Color::Green);
    feedbackText.setOutlineColor(sf::Color::Black);
    feedbackText.setOutlineThickness(1.f);
    centerText(feedbackText, width / 2, height - 50.f);

    // 4. Card creation (Grid)
    float cardW = 200.f;
    float cardH = 300.f;
    float gapX = 40.f;
    float gapY = 40.f;
    
    int cols = 4;
    float totalW = (cols * cardW) + ((cols - 1) * gapX);
    float startX = (width - totalW) / 2.f;
    float startY = 250.f;

    // Card creation
    for (size_t i = 0; i < availableCharacters.size(); ++i) {
        CharCard c;
        c.charIndex = i;
        c.isSelected = false;
        c.ownerId = 0;

        int col = i % cols;
        int row = i / cols;
        float x = startX + col * (cardW + gapX);
        float y = startY + row * (cardH + gapY);

        // Card background
        c.shape.setSize(sf::Vector2f(cardW, cardH));
        c.shape.setFillColor(sf::Color(40, 40, 40, 220));
        c.shape.setOutlineThickness(2.f);
        c.shape.setOutlineColor(sf::Color::White);
        c.shape.setPosition(x, y);

        // Sprite from ResourceManager
        std::string name = availableCharacters[i].getName();
        if (name.find("Pyra") != std::string::npos) {
            c.sprite.setTexture(ResourceManager::getInstance().getTexture("assets/images/pyra_python.png"));
        } else {
            c.sprite.setTexture(ResourceManager::getInstance().getTexture("assets/images/java_tron.png"));
        }

        sf::Vector2u ts = c.sprite.getTexture()->getSize();
        float scale = 180.f / std::max(ts.x, ts.y);
        c.sprite.setScale(scale, scale);
        c.sprite.setPosition(x + (cardW - (ts.x * scale)) / 2.f, y + 20.f);

        // Character name
        c.nameText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
        c.nameText.setString(name);
        c.nameText.setCharacterSize(18);
        c.nameText.setFillColor(sf::Color::White);
        
        sf::FloatRect b = c.nameText.getLocalBounds();
        c.nameText.setOrigin(b.left + b.width/2.f, b.top);
        c.nameText.setPosition(x + cardW/2.f, y + 230.f);

        // OWNER LABEL (Empty at the start)
        c.ownerText.setFont(ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF"));
        c.ownerText.setString(""); 
        c.ownerText.setCharacterSize(60); 
        c.ownerText.setStyle(sf::Text::Bold);
        
        cards.push_back(c);
    }
}
// --- CENTER TEXT ---
void CharacterSelectionState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}
// --- EVENT HANDLING ---
void CharacterSelectionState::handleInput(GameController& game, sf::Event& event) {
    // --- MOUSE (Hover) ---
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);
        for (size_t i = 0; i < cards.size(); ++i) {
            if (cards[i].shape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                hoveredIndex = i;
            }
        }
    }

    // --- KEYBOARD (Navigation) ---
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Right) hoveredIndex = (hoveredIndex + 1) % cards.size();
        else if (event.key.code == sf::Keyboard::Left) hoveredIndex = (hoveredIndex - 1 + cards.size()) % cards.size();
        else if (event.key.code == sf::Keyboard::Down) {
            if (hoveredIndex + 4 < cards.size()) hoveredIndex += 4;
        }
        else if (event.key.code == sf::Keyboard::Up) {
            if (hoveredIndex - 4 >= 0) hoveredIndex -= 4;
        }
    }

    // --- VALIDATION (Character choice) ---
    bool validate = false;
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);
        if (cards[hoveredIndex].shape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) validate = true;
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) validate = true;

    if (validate) {
        // If already taken, exit
        if (cards[hoveredIndex].isSelected) return; 

        Character chosen = availableCharacters[cards[hoveredIndex].charIndex];
        
        // --- VISUAL FEEDBACK: Display who joined which team ---
        std::string msg = "> " + chosen.getName() + " joins team P" + (currentPlayerTurn == 1 ? "1" : "2") + " ! <";
        feedbackText.setString(msg);
        centerText(feedbackText, game.window.getSize().x / 2.f, game.window.getSize().y - 50.f);

        // --- SELECTION LOGIC ---
        if (currentPlayerTurn == 1) {
            // Add to P1 vector
            p1Selection.push_back(chosen);
            
            // Update card (P1)
            cards[hoveredIndex].isSelected = true; 
            cards[hoveredIndex].ownerId = 1;
            
            cards[hoveredIndex].ownerText.setString("P1");
            cards[hoveredIndex].ownerText.setFillColor(sf::Color::Cyan);
            cards[hoveredIndex].ownerText.setOutlineColor(sf::Color::Black);
            cards[hoveredIndex].ownerText.setOutlineThickness(3.f);
            
            sf::FloatRect b = cards[hoveredIndex].ownerText.getLocalBounds();
            sf::Vector2f cPos = cards[hoveredIndex].shape.getPosition();
            sf::Vector2f cSize = cards[hoveredIndex].shape.getSize();
            cards[hoveredIndex].ownerText.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
            cards[hoveredIndex].ownerText.setPosition(cPos.x + cSize.x/2.f, cPos.y + cSize.y/2.f);

            // Transition to P2 if 3 characters are chosen
            if (p1Selection.size() == 3) {
                currentPlayerTurn = 2;
                instructionText.setString("PLAYER 2: Choose 3 champions");
                instructionText.setFillColor(sf::Color::Red);
                centerText(instructionText, game.window.getSize().x / 2.f, 120.f);
                
                // Short message to indicate it's P2's turn
                feedbackText.setString("Player 2's turn!");
                centerText(feedbackText, game.window.getSize().x / 2.f, game.window.getSize().y - 50.f);
            }
        } else {
            // Add to P2 vector
            p2Selection.push_back(chosen);
            
            // Update card (P2)
            cards[hoveredIndex].isSelected = true;
            cards[hoveredIndex].ownerId = 2;

            cards[hoveredIndex].ownerText.setString("P2");
            cards[hoveredIndex].ownerText.setFillColor(sf::Color::Red);
            cards[hoveredIndex].ownerText.setOutlineColor(sf::Color::Black);
            cards[hoveredIndex].ownerText.setOutlineThickness(3.f);

            sf::FloatRect b = cards[hoveredIndex].ownerText.getLocalBounds();
            sf::Vector2f cPos = cards[hoveredIndex].shape.getPosition();
            sf::Vector2f cSize = cards[hoveredIndex].shape.getSize();
            cards[hoveredIndex].ownerText.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
            cards[hoveredIndex].ownerText.setPosition(cPos.x + cSize.x/2.f, cPos.y + cSize.y/2.f);

            // START BATTLE if 3 characters are chosen by P2
            if (p2Selection.size() == 3) {
                std::cout << "Starting battle!" << std::endl;
                game.changeState(std::make_unique<BattleState>(game, p1Selection, p2Selection));
            }
        }
    }
}

void CharacterSelectionState::update(GameController& game) {
    for (size_t i = 0; i < cards.size(); ++i) {
        bool isHovered = (i == hoveredIndex);
        
        if (cards[i].isSelected) {
            // Cyan (P1) or Red (P2) border
            if (cards[i].ownerId == 1) cards[i].shape.setOutlineColor(sf::Color::Cyan);
            else if (cards[i].ownerId == 2) cards[i].shape.setOutlineColor(sf::Color::Red);
            
            cards[i].shape.setOutlineThickness(4.f);
            cards[i].shape.setFillColor(sf::Color(20, 20, 20, 230)); 
            cards[i].shape.setScale(0.95f, 0.95f);
        }
        else if (isHovered) {
            // Hover (Yellow)
            cards[i].shape.setFillColor(sf::Color(60, 60, 60, 240));
            cards[i].shape.setOutlineColor(sf::Color::Yellow);
            cards[i].shape.setOutlineThickness(4.f);
            cards[i].shape.setScale(1.05f, 1.05f);
        }
        else {
            // Normal
            cards[i].shape.setFillColor(sf::Color(40, 40, 40, 220));
            cards[i].shape.setOutlineColor(sf::Color::White);
            cards[i].shape.setOutlineThickness(2.f);
            cards[i].shape.setScale(1.f, 1.f);
        }
    }
}

void CharacterSelectionState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(instructionText);

    for (const auto& c : cards) {
        window.draw(c.shape);
        window.draw(c.sprite);
        window.draw(c.nameText);
        window.draw(c.ownerText); 
    }

    // Draw the feedback text last
    window.draw(feedbackText);
}