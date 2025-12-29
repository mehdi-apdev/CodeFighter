#include "../../include/BattleState.h"
#include "../../include/GameController.h"
#include "../../include/MenuState.h"
#include <iostream>
#include <string> // Important pour la recherche de texte
#include <memory> // Pour std::make_unique

BattleState::BattleState(GameController& game, std::vector<Character> p1Team, std::vector<Character> p2Team)
    : font(game.font),
      currentPhase(BattlePhase::InterTurn)
{
    // Remplissage J1
    for (const auto& c : p1Team) player1.addCharacter(c);
    // On peut ajouter ici une logique pour personnaliser le deck selon le perso, 
    // mais pour l'instant on garde le deck par défaut.
    for(int i=0; i<10; ++i) player1.addToDeck(new PythonStrike());
    player1.shuffleDeck();
    player1.initializeHand();

    // Remplissage J2
    for (const auto& c : p2Team) player2.addCharacter(c);
    for(int i=0; i<10; ++i) player2.addToDeck(new JavaException());
    player2.shuffleDeck();
    player2.initializeHand();

    game.playBattleMusic();
    initMatch();
}

// Initialisation du Match
void BattleState::initMatch() {
    activePlayer = &player1;
    inactivePlayer = &player2;

    // Fond
    if (!backgroundTexture.loadFromFile("assets/images/background_battle.png")) {
        std::cerr << "Erreur: Impossible de charger background_battle.png" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u texSize = backgroundTexture.getSize();
    backgroundSprite.setScale(1920.f / texSize.x, 1080.f / texSize.y);

    // Positions
    float screenW = 1920.f;
    float screenH = 1080.f;
    float contentWidth = 350.f;
    float pyraX = (screenW * 0.20f) - (contentWidth / 2.f);
    float javaX = (screenW * 0.80f) - (contentWidth / 2.f);
    float charY = screenH * 0.10f;

    // --- LOGIQUE DYNAMIQUE POUR L'IMAGE J1 ---
    std::string p1Name = player1.getActiveCharacter().getName();
    std::string p1Img;
    
    // Si le nom contient "Pyra", on met l'image de Pyra, sinon Java
    if (p1Name.find("Pyra") != std::string::npos) {
        p1Img = "assets/images/pyra_python.png";
        std::cout << "DEBUG: J1 joue Pyra -> Image Pyra chargee." << std::endl;
    } else {
        p1Img = "assets/images/java_tron.png";
        std::cout << "DEBUG: J1 joue Java -> Image Java chargee." << std::endl;
    }

    // Note : On utilise la variable 'pyraView' même si ce n'est pas Pyra (variable héritée)
    pyraView = std::make_unique<CharacterView>(
        player1.getActiveCharacter(),
        p1Img, 
        font, pyraX, charY, 0.4f
    );

    // --- LOGIQUE DYNAMIQUE POUR L'IMAGE J2 ---
    std::string p2Name = player2.getActiveCharacter().getName();
    std::string p2Img;

    if (p2Name.find("Pyra") != std::string::npos) {
        p2Img = "assets/images/pyra_python.png";
        std::cout << "DEBUG: J2 joue Pyra -> Image Pyra chargee." << std::endl;
    } else {
        p2Img = "assets/images/java_tron.png";
        std::cout << "DEBUG: J2 joue Java -> Image Java chargee." << std::endl;
    }

    javaTronView = std::make_unique<CharacterView>(
        player2.getActiveCharacter(),
        p2Img,
        font, javaX, charY, 0.4f
    );

    // Textes UI
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
        auto view = std::make_unique<CardView>(hand[i], font, startX + i * (cardWidth + gap), startY);
        handViews.push_back(std::move(view));
    }
}

void BattleState::handleInput(GameController& game, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        game.playMenuMusic();
        game.changeState(std::make_unique<MenuState>(game.font, 1920.f, 1080.f));
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
                sf::IntRect targetBounds;
                if (targetIsRight) targetBounds = sf::IntRect(1360, 78, 350, 400);
                else               targetBounds = sf::IntRect(210, 78, 350, 400);

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

        // --- GESTION DU SYSTÈME "TAG TEAM" AVEC IMAGES DYNAMIQUES ---

        // 1. Vérification Joueur 2 (Ennemi)
        if (!player2.getActiveCharacter().isAlive()) {
            if (player2.switchToNextCharacter()) {
                infoText.setString("Personnage KO ! Le suivant arrive !");
                
                // Choix dynamique de l'image pour le REMPLAÇANT J2
                std::string nextImg;
                if (player2.getActiveCharacter().getName().find("Pyra") != std::string::npos) 
                    nextImg = "assets/images/pyra_python.png";
                else 
                    nextImg = "assets/images/java_tron.png";

                javaTronView = std::make_unique<CharacterView>(
                    player2.getActiveCharacter(),
                    nextImg, // Image correcte
                    font, 1360.f, 108.f, 0.4f
                );
            } else {
                currentPhase = BattlePhase::CombatEnd;
                handViews.clear();
            }
        }

        // 2. Vérification Joueur 1 (Moi)
        else if (!player1.getActiveCharacter().isAlive()) {
            if (player1.switchToNextCharacter()) {
                infoText.setString("Personnage KO ! Renforts !");
                
                // Choix dynamique de l'image pour le REMPLAÇANT J1
                std::string nextImg;
                if (player1.getActiveCharacter().getName().find("Pyra") != std::string::npos) 
                    nextImg = "assets/images/pyra_python.png";
                else 
                    nextImg = "assets/images/java_tron.png";

                pyraView = std::make_unique<CharacterView>(
                    player1.getActiveCharacter(),
                    nextImg, // Image correcte
                    font, 210.f, 108.f, 0.4f
                );
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

    if (currentPhase == BattlePhase::CombatEnd) {
        bool p1Alive = !player1.isDefeated();
        std::string winnerName = p1Alive ? "JOUEUR 1" : "JOUEUR 2";
        promptText.setString("VICTOIRE FINAL : " + winnerName + " Gagne !");
        promptText.setFillColor(p1Alive ? sf::Color::Cyan : sf::Color::Red);
        infoText.setString("Appuyez sur ECHAP pour revenir au Menu.");
    }
    else if (currentPhase == BattlePhase::InterTurn) {
        promptText.setString("Tour de " + activePlayer->getActiveCharacter().getName());
        promptText.setFillColor(sf::Color::Yellow);
        infoText.setString(">>> CLIQUEZ POUR COMMENCER <<<");
    }
    else {
        std::string p1State = player1.getActiveCharacter().getName() + " (" + std::to_string(player1.getActiveCharacter().getHealth()) + ")";
        std::string p2State = player2.getActiveCharacter().getName() + " (" + std::to_string(player2.getActiveCharacter().getHealth()) + ")";
        promptText.setString(p1State + "   VS   " + p2State);
        promptText.setFillColor(sf::Color::White);

        if (selectedCard) infoText.setString("Ciblez l'adversaire !");
        else infoText.setString("Choisissez une carte.");
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