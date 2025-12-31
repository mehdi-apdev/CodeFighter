#include "../../include/states/BattleState.h"
#include "../../include/controllers/GameController.h"
#include "../../include/states/MenuState.h"
#include "core/ResourceManager.h"
#include <iostream>
#include <string>
#include <memory>

BattleState::BattleState(GameController& game, std::vector<Character> p1Team, std::vector<Character> p2Team)
    : currentPhase(BattlePhase::InterTurn)
{
    // Player 1 setup
    for (const auto& c : p1Team) player1.addCharacter(c);
    for(int i=0; i<10; ++i) player1.addToDeck(new PythonStrike());
    player1.shuffleDeck();
    player1.initializeHand();

    // Player 2 setup
    for (const auto& c : p2Team) player2.addCharacter(c);
    for(int i=0; i<10; ++i) player2.addToDeck(new JavaException());
    player2.shuffleDeck();
    player2.initializeHand();

    game.playBattleMusic();
    initMatch();
}

// Match initialization
void BattleState::initMatch() {
    activePlayer = &player1;
    inactivePlayer = &player2;

    // Background
    backgroundSprite.setTexture(ResourceManager::getInstance().getTexture("assets/images/background_battle.png"));
    sf::Vector2u texSize = backgroundSprite.getTexture()->getSize();
    backgroundSprite.setScale(1920.f / texSize.x, 1080.f / texSize.y);

    // Positions
    float screenW = 1920.f;
    float screenH = 1080.f;
    float contentWidth = 350.f;
    float pyraX = (screenW * 0.20f) - (contentWidth / 2.f);
    float javaX = (screenW * 0.80f) - (contentWidth / 2.f);
    float charY = screenH * 0.10f;

    std::string p1Name = player1.getActiveCharacter().getName();
    std::string p1Img = (p1Name.find("Pyra") != std::string::npos) ? "assets/images/pyra_python.png" : "assets/images/java_tron.png";
    
    pyraView = std::make_unique<CharacterView>(
        player1.getActiveCharacter(),
        p1Img, 
        pyraX, charY, 0.4f
    );

    std::string p2Name = player2.getActiveCharacter().getName();
    std::string p2Img = (p2Name.find("Pyra") != std::string::npos) ? "assets/images/pyra_python.png" : "assets/images/java_tron.png";

    javaTronView = std::make_unique<CharacterView>(
        player2.getActiveCharacter(),
        p2Img,
        javaX, charY, 0.4f
    );

    // UI Texts
    sf::Font& font = ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF");
    promptText.setFont(font);
    promptText.setCharacterSize(50);
    promptText.setFillColor(sf::Color::White);

    infoText.setFont(font);
    infoText.setCharacterSize(30);
    infoText.setFillColor(sf::Color::Yellow);

    currentPhase = BattlePhase::InterTurn;
    updateHandViews();
}

void BattleState::updateHandViews() {
    handViews.clear();
    selectedCard = nullptr;

    if (currentPhase == BattlePhase::InterTurn || currentPhase == BattlePhase::CombatEnd) {
        return;
    }

    const auto& hand = activePlayer->getHand();
    if (hand.empty()) return;

    float screenW = 1920.f;
    float screenH = 1080.f;
    float cardWidth = 120.f;
    float gap = 20.f;
    float totalWidth = (hand.size() * cardWidth) + ((hand.size() - 1) * gap);
    float startX = (screenW - totalWidth) / 2.0f;
    float startY = screenH - 220.f;

    for (size_t i = 0; i < hand.size(); ++i) {
        auto view = std::make_unique<CardView>(hand[i], startX + i * (cardWidth + gap), startY);
        handViews.push_back(std::move(view));
    }
}

void BattleState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        game.playMenuMusic();
        game.changeState(std::make_unique<MenuState>(1920.f, 1080.f)); // Signature updated
        return;
    }

    if (currentPhase == BattlePhase::InterTurn) {
        if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
            currentPhase = BattlePhase::WaitingForInput;
            updateHandViews();
        }
    }
    else if (currentPhase == BattlePhase::WaitingForInput) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);

            for (auto& cardView : handViews) {
                if (cardView->isClicked(mousePos)) {
                    if (selectedCard) selectedCard->highlight(false);
                    selectedCard = cardView.get();
                    selectedCard->highlight(true);
                    return;
                }
            }

            if (selectedCard) {
                bool targetIsRight = (inactivePlayer == &player2);
                sf::IntRect targetBounds = targetIsRight ? sf::IntRect(1360, 78, 350, 400) : sf::IntRect(210, 78, 350, 400);

                if (targetBounds.contains(mousePos)) {
                    Character& attacker = activePlayer->getActiveCharacter();
                    Character& defender = inactivePlayer->getActiveCharacter();
                    selectedCard->getAbility()->execute(attacker, defender);
                    selectedCard->highlight(false);
                    selectedCard = nullptr;
                    currentPhase = BattlePhase::TurnTransition;
                }
            }
        }
    }
}

void BattleState::update(GameController& game) {
    if (currentPhase != BattlePhase::CombatEnd) {

        if (!player2.getActiveCharacter().isAlive()) {
            if (player2.switchToNextCharacter()) {
                infoText.setString("Character KO! Next one arrives!");
                std::string nextImg = (player2.getActiveCharacter().getName().find("Pyra") != std::string::npos) ? "assets/images/pyra_python.png" : "assets/images/java_tron.png";
                javaTronView = std::make_unique<CharacterView>(player2.getActiveCharacter(), nextImg, 1360.f, 108.f, 0.4f);
            } else {
                currentPhase = BattlePhase::CombatEnd;
                handViews.clear();
            }
        }
        else if (!player1.getActiveCharacter().isAlive()) {
            if (player1.switchToNextCharacter()) {
                infoText.setString("Character KO! Reinforcements!");
                std::string nextImg = (player1.getActiveCharacter().getName().find("Pyra") != std::string::npos) ? "assets/images/pyra_python.png" : "assets/images/java_tron.png";
                pyraView = std::make_unique<CharacterView>(player1.getActiveCharacter(), nextImg, 210.f, 108.f, 0.4f);
            } else {
                currentPhase = BattlePhase::CombatEnd;
                handViews.clear();
            }
        }
        else if (currentPhase == BattlePhase::TurnTransition) {
            std::swap(activePlayer, inactivePlayer);
            currentPhase = BattlePhase::InterTurn;
            updateHandViews();
        }
    }

    float screenW = 1920.f;
    sf::Font& font = ResourceManager::getInstance().getFont("assets/fonts/ARIAL.TTF");
    promptText.setFont(font); // Re-set font just in case
    infoText.setFont(font);

    if (currentPhase == BattlePhase::CombatEnd) {
        bool p1Alive = !player1.isDefeated();
        std::string winnerName = p1Alive ? "JOUEUR 1" : "JOUEUR 2";
        promptText.setString("FINAL VICTORY: " + winnerName + " Wins!");
        promptText.setFillColor(p1Alive ? sf::Color::Cyan : sf::Color::Red);
        infoText.setString("Press ESC to return to Menu.");
    }
    else if (currentPhase == BattlePhase::InterTurn) {
        promptText.setString("Turn of " + activePlayer->getActiveCharacter().getName());
        promptText.setFillColor(sf::Color::Yellow);
        infoText.setString(">>> CLICK TO START <<<");
    }
    else {
        std::string p1State = player1.getActiveCharacter().getName() + " (" + std::to_string(player1.getActiveCharacter().getHealth()) + ")";
        std::string p2State = player2.getActiveCharacter().getName() + " (" + std::to_string(player2.getActiveCharacter().getHealth()) + ")";
        promptText.setString(p1State + "   VS   " + p2State);
        promptText.setFillColor(sf::Color::White);

        if (selectedCard) infoText.setString("Target the opponent!");
        else infoText.setString("Choose a card.");
    }

    centerText(promptText, screenW / 2.0f, 50.f);
    centerText(infoText, screenW / 2.0f, 1080.f - 250.f);
}

void BattleState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    pyraView->draw(window);
    javaTronView->draw(window);
    for (const auto& card : handViews) card->draw(window);
    window.draw(promptText);
    window.draw(infoText);
}

void BattleState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}