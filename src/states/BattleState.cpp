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
    //add characters to players and setup decks with a lambda to avoid code duplication
    auto setupPlayerDeck = [](Player& player, const std::vector<Character>& team) {
        for (const auto& c : team) {
            player.addCharacter(c);
            
            std::string name = c.getName();
            
            for(int i = 0; i < 5; ++i) {
                if (name.find("Pyra") != std::string::npos) {
                    player.addToDeck(new PythonStrike()); 
                } 
                else if (name.find("JavaScript") != std::string::npos) {
                    player.addToDeck(new Undefined());
                }
                else if (name.find("Java") != std::string::npos) {
                    player.addToDeck(new JavaException()); // Ou une classe JavaTron spécifique
                }
                else if (name.find("SQL") != std::string::npos) {
                    player.addToDeck(new SelectTarget());
                }
                else if (name.find("CSS") != std::string::npos) {
                    player.addToDeck(new Flexbox());
                }
                else if (name.find("Bash") != std::string::npos) {
                    player.addToDeck(new Pipe());
                }
                else if (name.find("Go") != std::string::npos) {
                    player.addToDeck(new Goroutine());
                }
                else if (name.find("PHP") != std::string::npos) {
                    player.addToDeck(new IncludeOnce());
                }
            }
        }
        player.shuffleDeck();
        player.initializeHand();
    };

    setupPlayerDeck(player1, p1Team);
    setupPlayerDeck(player2, p2Team);
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
std::string p1Img;

if (p1Name.find("Pyra") != std::string::npos) p1Img = "assets/images/pyra_python.png";
else if (p1Name.find("Java") != std::string::npos) p1Img = "assets/images/java_tron.png";
else if (p1Name.find("SQL") != std::string::npos) p1Img = "assets/images/Sql.png";
else if (p1Name.find("CSS") != std::string::npos) p1Img = "assets/images/css.png";
else if (p1Name.find("Bash") != std::string::npos) p1Img = "assets/images/bash.jpg";

else if (p1Name.find("Go") != std::string::npos) p1Img = "assets/images/go.png";
else if (p1Name.find("JavaScript") != std::string::npos) p1Img = "assets/images/javascript.png";
else if (p1Name.find("PHP") != std::string::npos) p1Img = "assets/images/php.png";

if (!p1Img.empty()) {
    pyraView = std::make_unique<CharacterView>(
        player1.getActiveCharacter(),
        p1Img, 
        pyraX, charY, 0.4f
    );
}

    

    std::string p2Name = player2.getActiveCharacter().getName();
std::string p2Img;

if (p2Name.find("Pyra") != std::string::npos) p2Img = "assets/images/pyra_python.png";
else if (p2Name.find("Java") != std::string::npos) p2Img = "assets/images/java_tron.png";
else if (p2Name.find("SQL") != std::string::npos) p2Img = "assets/images/Sql.png";
else if (p2Name.find("CSS") != std::string::npos) p2Img = "assets/images/css.png";
else if (p2Name.find("Bash") != std::string::npos) p2Img = "assets/images/bash.jpg";
else if (p2Name.find("Go") != std::string::npos) p2Img = "assets/images/go.png";
else if (p2Name.find("JavaScript") != std::string::npos) p2Img = "assets/images/javascript.png";
else if (p2Name.find("PHP") != std::string::npos) p2Img = "assets/images/php.png";

if (!p2Img.empty()) {
    javaTronView = std::make_unique<CharacterView>(
        player2.getActiveCharacter(),
        p2Img,
        javaX, charY, 0.4f
    );
}

   

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

    // --- Dev Keys for testing Status Effects ---
    if (event.type == sf::Event::KeyPressed) {
        Character& targetCharacter = activePlayer->getActiveCharacter();
        if (event.key.code == sf::Keyboard::P) {
            // Apply POISON (3 turns, 10 dmg)
            targetCharacter.addStatus({StatusType::POISON, 3, 10});
            std::cout << " > [DEV] Applied POISON to " << targetCharacter.getName() << std::endl;
        } else if (event.key.code == sf::Keyboard::B) {
            // Apply BLOCK (1 turn, value 50 for 50 damage reduction)
            targetCharacter.addStatus({StatusType::BLOCK, 1, 50});
            std::cout << " > [DEV] Applied BLOCK to " << targetCharacter.getName() << std::endl;
        } else if (event.key.code == sf::Keyboard::S) {
            // Apply STUN (1 turn)
            targetCharacter.addStatus({StatusType::STUN, 1, 0});
            std::cout << " > [DEV] Applied STUN to " << targetCharacter.getName() << std::endl;
        }
    }
    // --- End Dev Keys ---

    if (currentPhase == BattlePhase::InterTurn) {
        if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
            currentPhase = BattlePhase::WaitingForInput;
            updateHandViews();
        }
    }
    else if (currentPhase == BattlePhase::WaitingForInput) {
        if (activePlayer->getActiveCharacter().isStunned()) {
            infoText.setString(activePlayer->getActiveCharacter().getName() + " is stunned and cannot act!");
            return; // Prevent any further input processing for the stunned character
        }
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

        // Fonction locale pour déterminer l'image
auto getNextImgPath = [](const std::string& name) -> std::string {
    if (name.find("Pyra") != std::string::npos) return "assets/images/pyra_python.png";
    if (name.find("JavaScript") != std::string::npos) return "assets/images/javascript.png"; // Attention: Vérifier avant Java
    if (name.find("Java") != std::string::npos) return "assets/images/java_tron.png";
    if (name.find("SQL") != std::string::npos) return "assets/images/Sql.png";
    if (name.find("CSS") != std::string::npos) return "assets/images/css.png";
    if (name.find("Bash") != std::string::npos) return "assets/images/bash.png";
    if (name.find("Go") != std::string::npos) return "assets/images/go.png";
    if (name.find("PHP") != std::string::npos) return "assets/images/php.png";
    return "assets/images/java_tron.png"; // Image par défaut
};

if (!player2.getActiveCharacter().isAlive()) {
    if (player2.switchToNextCharacter()) {
        infoText.setString("Character KO! Next one arrives!");
        std::string nextImg = getNextImgPath(player2.getActiveCharacter().getName());
        javaTronView = std::make_unique<CharacterView>(player2.getActiveCharacter(), nextImg, 1360.f, 108.f, 0.4f);
    } else {
        currentPhase = BattlePhase::CombatEnd;
        handViews.clear();
    }
}
else if (!player1.getActiveCharacter().isAlive()) {
    if (player1.switchToNextCharacter()) {
        infoText.setString("Character KO! Reinforcements!");
        std::string nextImg = getNextImgPath(player1.getActiveCharacter().getName());
        pyraView = std::make_unique<CharacterView>(player1.getActiveCharacter(), nextImg, 210.f, 108.f, 0.4f);
    } else {
        currentPhase = BattlePhase::CombatEnd;
        handViews.clear();
    }
}
        else if (currentPhase == BattlePhase::TurnTransition) {
            std::swap(activePlayer, inactivePlayer);
            activePlayer->getActiveCharacter().updateStatus(); // Update status for the new active player
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