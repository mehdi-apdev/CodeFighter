#include "../../include/BattleState.h"
#include "../../include/GameController.h"

#include "../../include/MenuState.h" 
#include <iostream>
#include <cmath>

BattleState::BattleState(GameController& game)
    : font(game.font), currentPhase(BattlePhase::InterTurn)
{
    game.playBattleMusic();
    initMatch();
}


void BattleState::initMatch() {
    // Setup Player 1
    player1.addCharacter(Character(1, "Pyra (Core)", 100, "Le moteur principal."));
    player1.addCharacter(Character(2, "Pyra (Venv)", 80, "L'environnement virtuel."));
    player1.addCharacter(Character(3, "Pyra (GIL)", 120, "Le Global Interpreter Lock."));
    for(int i=0; i<10; ++i) player1.addToDeck(new PythonStrike());
    player1.shuffleDeck();
    player1.initializeHand();

    // Setup Player 2
    player2.addCharacter(Character(1, "Java (JVM)", 90, "La machine virtuelle."));
    player2.addCharacter(Character(2, "Java (GC)", 100, "Le Garbage Collector."));
    player2.addCharacter(Character(3, "Java (JIT)", 70, "Le compilateur Just-In-Time."));
    for(int i=0; i<10; ++i) player2.addToDeck(new JavaException());
    player2.shuffleDeck();
    player2.initializeHand();

    activePlayer = &player1;
    inactivePlayer = &player2;

    if (!backgroundTexture.loadFromFile("assets/images/background_battle.png")) {}
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u texSize = backgroundTexture.getSize();
    backgroundSprite.setScale(1920.f / texSize.x, 1080.f / texSize.y);


    float screenW = 1920.f;
    float screenH = 1080.f;
    float contentWidth = 350.f;
    float pyraX = (screenW * 0.20f) - (contentWidth / 2.f);
    float javaX = (screenW * 0.80f) - (contentWidth / 2.f);
    float charY = screenH * 0.10f;


    pyraView = std::make_unique<CharacterView>(*player1.getActiveCharacter(), "assets/images/pyra_python.png", font, pyraX, charY, 0.4f);
    javaTronView = std::make_unique<CharacterView>(*player2.getActiveCharacter(), "assets/images/java_tron.png", font, javaX, charY, 0.4f);

    promptText.setFont(font); promptText.setCharacterSize(50); promptText.setFillColor(sf::Color::White);
    infoText.setFont(font); infoText.setCharacterSize(30); infoText.setFillColor(sf::Color::Yellow);

    reshuffleText.setFont(font);
    reshuffleText.setString("RECYCLAGE DU DECK !");
    reshuffleText.setCharacterSize(60);
    reshuffleText.setFillColor(sf::Color::Cyan);
    reshuffleText.setStyle(sf::Text::Bold);
    centerText(reshuffleText, screenW / 2.0f, screenH / 2.0f);

    currentPhase = BattlePhase::InterTurn;
    updateHandViews();
}

void BattleState::updateHandViews() {
    handViews.clear();
    selectedCard = nullptr;

    selectedCardIndex = -1;

    if (currentPhase == BattlePhase::InterTurn || currentPhase == BattlePhase::CombatEnd) return;

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
        auto view = std::make_unique<CardView>(hand[i], font, startX + i * (cardWidth + gap), startY);
        handViews.push_back(std::move(view));
    }
}

void BattleState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        game.playMenuMusic();
        game.changeState(std::make_unique<MenuState>(game.font, 1920.f, 1080.f));
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


            // A. Selection
            for (size_t i = 0; i < handViews.size(); ++i) {
                if (handViews[i]->isClicked(mousePos)) {
                    if (selectedCard) selectedCard->highlight(false);
                    selectedCard = handViews[i].get();
                    selectedCardIndex = static_cast<int>(i);
                    selectedCard->highlight(true);
                    return;
                }
            }

            // B. Attack
            if (selectedCard && selectedCardIndex != -1) {
                bool targetIsRight = (inactivePlayer == &player2);
                sf::IntRect targetBounds = targetIsRight ? sf::IntRect(1360, 78, 350, 400) : sf::IntRect(210, 78, 350, 400);

                if (targetBounds.contains(mousePos)) {
                    Character* attacker = activePlayer->getActiveCharacter();
                    Character* defender = inactivePlayer->getActiveCharacter();

                    if (attacker && defender) {
                        // 1. Execute effect
                        selectedCard->getAbility()->execute(*attacker, *defender);

                        // 2. MODIFIED: Play the card and it disappears (no immediate draw)
                        activePlayer->playCard(selectedCardIndex);

                        // 3. Reset UI & Next turn transition
                        selectedCard->highlight(false);
                        selectedCard = nullptr;
                        selectedCardIndex = -1;
                        
                        // The hand is not visually updated immediately because we change phase
                        // But the card is indeed removed from player data.
                        currentPhase = BattlePhase::TurnTransition;
                    }
                }
            }
        }
    }
}

void BattleState::update(GameController& game) {

    float dt = animClock.restart().asSeconds();
    if (reshuffleDisplayTime > 0.0f) {
        reshuffleDisplayTime -= dt;
        float scale = 1.0f + (sin(reshuffleDisplayTime * 10) * 0.1f);
        reshuffleText.setScale(scale, scale);
    }

    if (currentPhase != BattlePhase::CombatEnd) {
        // Shuffle Animation
        if (activePlayer->hasReshuffled()) {
            reshuffleDisplayTime = 2.0f;
            reshuffleText.setString("DECK DE " + activePlayer->getActiveCharacter()->getName() + "\nRECYCLE !");
            centerText(reshuffleText, 1920.f / 2.0f, 1080.f / 2.0f);
            activePlayer->clearReshuffleFlag();
        }
        if (inactivePlayer->hasReshuffled()) {
            reshuffleDisplayTime = 2.0f;
            reshuffleText.setString("DECK DE " + inactivePlayer->getActiveCharacter()->getName() + "\nRECYCLE !");
            centerText(reshuffleText, 1920.f / 2.0f, 1080.f / 2.0f);
            inactivePlayer->clearReshuffleFlag();
        }

        // Death Handling / Tag Team
        if (!pyraView->getCharacter().isAlive()) {
            Character* nextChar = player1.getActiveCharacter();
            if (nextChar) {
                pyraView = std::make_unique<CharacterView>(*nextChar, "assets/images/pyra_python.png", font, pyraView->getPosition().x, pyraView->getPosition().y, 0.4f);
                infoText.setString("Joueur 1 envoie " + nextChar->getName() + " !");
            }
        }
        if (!javaTronView->getCharacter().isAlive()) {
            Character* nextChar = player2.getActiveCharacter();
            if (nextChar) {
                javaTronView = std::make_unique<CharacterView>(*nextChar, "assets/images/java_tron.png", font, javaTronView->getPosition().x, javaTronView->getPosition().y, 0.4f);
                infoText.setString("Joueur 2 envoie " + nextChar->getName() + " !");
            }
        }

        if (player1.isDefeated() || player2.isDefeated()) {
            currentPhase = BattlePhase::CombatEnd;
            handViews.clear();
        }
        // MODIFIED: Turn Transition
        else if (currentPhase == BattlePhase::TurnTransition) {
            std::swap(activePlayer, inactivePlayer);
            
            // NEW: Here we refill the active player's hand (up to 7 cards)
            activePlayer->refillHand(7);
            
            currentPhase = BattlePhase::InterTurn;
            updateHandViews();
        }
    }


    // UI Updates
    float screenW = 1920.f;
    if (currentPhase == BattlePhase::CombatEnd) {
        bool p1Win = !player1.isDefeated();
        promptText.setString(p1Win ? "VICTOIRE : JOUEUR 1" : "VICTOIRE : JOUEUR 2");
        promptText.setFillColor(p1Win ? sf::Color::Green : sf::Color::Red);
        infoText.setString("Appuyez sur ECHAP pour revenir au Menu.");
    }
    else if (currentPhase == BattlePhase::InterTurn) {
        promptText.setString("Tour de " + activePlayer->getActiveCharacter()->getName());
        promptText.setFillColor(sf::Color::Yellow);
        infoText.setString(">>> CLIQUEZ POUR COMMENCER <<<");
    }
    else {

        Character* p1Char = player1.getActiveCharacter();
        Character* p2Char = player2.getActiveCharacter();
        promptText.setString(p1Char->getName() + " (" + std::to_string(p1Char->getHealth()) + ")   VS   " + p2Char->getName() + " (" + std::to_string(p2Char->getHealth()) + ")");
        promptText.setFillColor(sf::Color::White);
        infoText.setString(selectedCard ? "Ciblez l'adversaire !" : "Choisissez une carte.");
    }
    centerText(promptText, screenW / 2.0f, 100.f);
    centerText(infoText, screenW / 2.0f, 1080.f - 300.f);
}

void BattleState::render(GameController& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    pyraView->draw(window);
    javaTronView->draw(window);
    for (const auto& card : handViews) card->draw(window);
    window.draw(promptText);
    window.draw(infoText);

    if (reshuffleDisplayTime > 0.0f) {
        sf::RectangleShape dim(sf::Vector2f(1920.f, 1080.f));
        dim.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(dim);
        window.draw(reshuffleText);
    }

}

void BattleState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}