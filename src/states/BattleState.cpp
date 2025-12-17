#include "../../include/BattleState.h"
#include "../../include/GameController.h"
#include "../../include/MenuState.h" // Pour revenir au menu � la fin (optionnel)
#include <iostream>

// Constructeur
BattleState::BattleState(GameController& game)
    : font(game.font), // On utilise la police charg�e dans le Controller
      currentPhase(BattlePhase::InterTurn)
{
    initMatch();
}

// Initialisation du Match (Setup des joueurs et des vues)
void BattleState::initMatch() {
    // 1. Setup Joueur 1 (Pyra)
    Character pyra(1, "Pyra (Python)", 100, "Le code dynamique.");
    player1.addCharacter(pyra);
    for(int i=0; i<10; ++i) player1.addToDeck(new PythonStrike());
    player1.shuffleDeck();
    player1.initializeHand();

    // 2. Setup Joueur 2 (Java-Tron)
    Character javaTron(2, "Java-Tron", 80, "La machine virtuelle.");
    player2.addCharacter(javaTron);
    for(int i=0; i<10; ++i) player2.addToDeck(new JavaException());
    player2.shuffleDeck();
    player2.initializeHand();

    // 3. D�finir qui commence
    activePlayer = &player1;
    inactivePlayer = &player2;

    // --- SETUP ARRI�RE-PLAN DE COMBAT ---
    if (!backgroundTexture.loadFromFile("assets/images/background_battle.png")) {
        // Fallback
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Scaling pour 1920x1080 (valeurs cod�es en dur ou r�cup�r�es de window.getSize())
    sf::Vector2u texSize = backgroundTexture.getSize();
    backgroundSprite.setScale(1920.f / texSize.x, 1080.f / texSize.y);

    // 4. Initialisation Vues (Centrage 1920x1080)
    float screenW = 1920.f;
    float screenH = 1080.f;
    float contentWidth = 350.f;

    // Positionnement Sym�trique
    float pyraX = (screenW * 0.20f) - (contentWidth / 2.f);
    float javaX = (screenW * 0.80f) - (contentWidth / 2.f);
    float charY = screenH * 0.10f;

    pyraView = std::make_unique<CharacterView>(
        player1.getCharacter(0), 
        "assets/images/pyra_python.png",
        font, 
        pyraX, 
        charY, 
        0.4f
    );

    javaTronView = std::make_unique<CharacterView>(
        player2.getCharacter(0), 
        "assets/images/java_tron.png",
        font, 
        javaX, 
        charY, 
        0.4f
    );
    // 5. Textes
    promptText.setFont(font);
    promptText.setCharacterSize(50);
    promptText.setFillColor(sf::Color::White);

    infoText.setFont(font);
    infoText.setCharacterSize(30);
    infoText.setFillColor(sf::Color::Yellow);

    // On commence en InterTurn (Cartes cach�es)
    currentPhase = BattlePhase::InterTurn;
    updateHandViews();
}

// Gestion de l'affichage de la main
void BattleState::updateHandViews() {
    handViews.clear();
    selectedCard = nullptr;

    // Si on est en transition ou fin, on n'affiche rien
    if (currentPhase == BattlePhase::InterTurn || currentPhase == BattlePhase::CombatEnd) {
        return;
    }

    const auto& hand = activePlayer->getHand();
    if (hand.empty()) return;

    // Centrage des cartes en bas
    float screenW = 1920.f;
    float screenH = 1080.f;
    float cardWidth = 120.f;
    float gap = 20.f;

    float totalWidth = (hand.size() * cardWidth) + ((hand.size() - 1) * gap);
    float startX = (screenW - totalWidth) / 2.0f;
    float startY = screenH - 220.f;

    for (size_t i = 0; i < hand.size(); ++i) {
        // On cr�e la vue pour chaque carte
        auto view = std::make_unique<CardView>(hand[i], font, startX + i * (cardWidth + gap), startY);
        handViews.push_back(std::move(view));
    }
}

// Gestion des Inputs (Clics Souris & Clavier)
void BattleState::handleInput(GameController& game, sf::Event& event) {
    // Touche ECHAP pour quitter ou revenir au menu
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (currentPhase == BattlePhase::CombatEnd) {
            // Retour au menu principal
            game.changeState(std::make_unique<MenuState>(game.font, 1920.f, 1080.f));
        } else {
            game.changeState(std::make_unique<MenuState>(game.font, 1920.f, 1080.f));
        }
    }

    // --- PHASE 1 : INTER-TOUR (Ecran d'attente) ---
    if (currentPhase == BattlePhase::InterTurn) {
        // Clic ou Espace pour commencer le tour
        if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
            currentPhase = BattlePhase::WaitingForInput;
            updateHandViews(); // Affiche les cartes du joueur actif
        }
    }

    // --- PHASE 2 : TOUR DE JEU (S�lection & Attaque) ---
    else if (currentPhase == BattlePhase::WaitingForInput) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);

            // A. S�lection d'une carte dans la main
            for (auto& cardView : handViews) {
                if (cardView->isClicked(mousePos)) {
                    if (selectedCard) selectedCard->highlight(false);
                    selectedCard = cardView.get();
                    selectedCard->highlight(true);
                    return; // On a g�r� le clic
                }
            }

            // B. Validation sur l'adversaire (Si une carte est s�lectionn�e)
            if (selectedCard) {
                // D�terminer la zone de clic de l'adversaire
                // Si P2 est la cible (activePlayer est P1), il est � droite (x ~1360)
                // Si P1 est la cible (activePlayer est P2), il est � gauche (x ~210)
                bool targetIsRight = (inactivePlayer == &player2);

                // Coordonn�es ajust�es pour 1920x1080 (voir calculs pr�c�dents)
                sf::IntRect targetBounds;
                if (targetIsRight) targetBounds = sf::IntRect(1360, 78, 350, 400);
                else               targetBounds = sf::IntRect(210, 78, 350, 400);

                if (targetBounds.contains(mousePos)) {
                    // EX�CUTION DE L'ATTAQUE
                    Character& attacker = activePlayer->getCharacter(0);
                    Character& defender = inactivePlayer->getCharacter(0);

                    selectedCard->getAbility()->execute(attacker, defender);

                    // Fin du tour
                    selectedCard->highlight(false);
                    selectedCard = nullptr;
                    currentPhase = BattlePhase::TurnTransition;
                }
            }
        }
    }
}

// Logique de Mise � Jour (Update)
void BattleState::update(GameController& game) {
    // Si le combat est fini, on ne met � jour que les textes (partie basse de la fonction)
    if (currentPhase != BattlePhase::CombatEnd) {

        // 1. V�rification des Morts
        bool p1Dead = !player1.getCharacter(0).isAlive();
        bool p2Dead = !player2.getCharacter(0).isAlive();

        if (p1Dead || p2Dead) {
            currentPhase = BattlePhase::CombatEnd;
            handViews.clear(); // On cache les cartes
        }

        // 2. Transition de Tour
        else if (currentPhase == BattlePhase::TurnTransition) {
            // �change des joueurs
            std::swap(activePlayer, inactivePlayer);

            // Passage en mode cach�
            currentPhase = BattlePhase::InterTurn;
            updateHandViews();
        }
    }

    // --- MISE � JOUR UI & TEXTES ---
    float screenW = 1920.f;

    if (currentPhase == BattlePhase::CombatEnd) {
        bool p1Alive = player1.getCharacter(0).isAlive();
        std::string winnerName = p1Alive ? player1.getCharacter(0).getName() : player2.getCharacter(0).getName();

        promptText.setString("VICTOIRE : " + winnerName + " Gagne !");
        promptText.setFillColor(p1Alive ? sf::Color::Green : sf::Color::Red);
        infoText.setString("Appuyez sur ECHAP pour revenir au Menu.");
    }
    else if (currentPhase == BattlePhase::InterTurn) {
        promptText.setString("C'est au tour de " + activePlayer->getCharacter(0).getName());
        promptText.setFillColor(sf::Color::Yellow);
        infoText.setString(">>> CLIQUEZ POUR COMMENCER <<<");
    }
    else {
        // En Jeu
        std::string p1State = player1.getCharacter(0).getName() + " (" + std::to_string(player1.getCharacter(0).getHealth()) + ")";
        std::string p2State = player2.getCharacter(0).getName() + " (" + std::to_string(player2.getCharacter(0).getHealth()) + ")";

        promptText.setString(p1State + "   VS   " + p2State);
        promptText.setFillColor(sf::Color::White);

        if (selectedCard)
            infoText.setString("Ciblez l'adversaire avec " + selectedCard->getAbility()->getName());
        else
            infoText.setString("Choisissez une carte.");
    }

    // Centrage constant
    centerText(promptText, screenW / 2.0f, 100.f);
    centerText(infoText, screenW / 2.0f, 1080.f - 300.f);
}

// Rendu (Draw)
void BattleState::render(GameController& game, sf::RenderWindow& window) {
    // Note: Le game.render() a d�j� effac� l'�cran et dessin� le backgroundSprite.
    // On dessine juste par dessus.

    window.draw(backgroundSprite);

    pyraView->draw(window);
    javaTronView->draw(window);

    // Dessin de la main (si visible)
    for (const auto& card : handViews) {
        card->draw(window);
    }

    window.draw(promptText);
    window.draw(infoText);
}

// Helper pour centrer
void BattleState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}
