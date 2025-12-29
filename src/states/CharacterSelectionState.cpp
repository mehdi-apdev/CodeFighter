#include "../../include/CharacterSelectionState.h"
#include "../../include/GameController.h"
#include "../../include/BattleState.h" // INDISPENSABLE pour lancer le combat
#include <iostream>


// --- CONSTRUCTEUR ---
CharacterSelectionState::CharacterSelectionState(GameController& game) 
    : font(game.font) 
{
    initRoster();
    loadAssets((float)game.window.getSize().x, (float)game.window.getSize().y);
    initUI((float)game.window.getSize().x, (float)game.window.getSize().y);
}
// --- MÉTHODES PRIVÉES ---
void CharacterSelectionState::initRoster() {
    availableCharacters.emplace_back(1, "Pyra (Core)", 100, "Le moteur principal.");
    availableCharacters.emplace_back(2, "Pyra (Venv)", 80, "L'environnement virtuel.");
    availableCharacters.emplace_back(3, "Pyra (GIL)", 120, "Le Global Interpreter Lock.");
    availableCharacters.emplace_back(4, "Java (JVM)", 90, "La machine virtuelle.");
    availableCharacters.emplace_back(5, "Java (GC)", 100, "Le Garbage Collector.");
    availableCharacters.emplace_back(6, "Java (JIT)", 70, "Le compilateur Just-In-Time.");
}
// --- CHARGEMENT DES ASSETS ---
void CharacterSelectionState::loadAssets(float width, float height) {
    if (!backgroundTexture.loadFromFile("assets/images/Perso_Menu.png")) {
        std::cerr << "Erreur: Impossible de charger menu_bg.png" << std::endl;
    } else {
        backgroundSprite.setTexture(backgroundTexture);
        sf::Vector2u texSize = backgroundTexture.getSize();
        backgroundSprite.setScale(width / texSize.x, height / texSize.y);
        backgroundSprite.setColor(sf::Color(150, 150, 150)); 
    }

    if (!pyraTexture.loadFromFile("assets/images/pyra_python.png")) std::cerr << "Erreur Pyra texture" << std::endl;
    if (!javaTexture.loadFromFile("assets/images/java_tron.png")) std::cerr << "Erreur Java texture" << std::endl;
}
// --- INITIALISATION DE L'UI ---
void CharacterSelectionState::initUI(float width, float height) {
    // 1. Titre
    titleText.setFont(font);
    titleText.setString("SELECTION DES CHAMPIONS");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);
    centerText(titleText, width / 2, 50);

    // 2. Instructions
    instructionText.setFont(font);
    instructionText.setString("JOUEUR 1 : Choisissez 3 champions");
    instructionText.setCharacterSize(36);
    instructionText.setFillColor(sf::Color::Cyan);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(2.f);
    centerText(instructionText, width / 2, 120);

    // 3. Feedback (Le texte de confirmation en bas)
    feedbackText.setFont(font);
    feedbackText.setString(""); // Vide au début
    feedbackText.setCharacterSize(30);
    feedbackText.setFillColor(sf::Color::Green);
    feedbackText.setOutlineColor(sf::Color::Black);
    feedbackText.setOutlineThickness(1.f);
    centerText(feedbackText, width / 2, height - 50.f);

    // 4. Création des cartes (Grille)
    float cardW = 200.f;
    float cardH = 300.f;
    float gapX = 40.f;
    float gapY = 40.f;
    
    int cols = 4;
    float totalW = (cols * cardW) + ((cols - 1) * gapX);
    float startX = (width - totalW) / 2.f;
    float startY = 250.f;
// Création des cartes
    for (size_t i = 0; i < availableCharacters.size(); ++i) {
        CharCard c;
        c.charIndex = i;
        c.isSelected = false;
        c.ownerId = 0;

        int col = i % cols;
        int row = i / cols;
        float x = startX + col * (cardW + gapX);
        float y = startY + row * (cardH + gapY);

        // Fond de carte
        c.shape.setSize(sf::Vector2f(cardW, cardH));
        c.shape.setFillColor(sf::Color(40, 40, 40, 220));
        c.shape.setOutlineThickness(2.f);
        c.shape.setOutlineColor(sf::Color::White);
        c.shape.setPosition(x, y);

        // Sprite
        std::string name = availableCharacters[i].getName();
        if (name.find("Pyra") != std::string::npos) c.sprite.setTexture(pyraTexture);
        else c.sprite.setTexture(javaTexture);

        sf::Vector2u ts = c.sprite.getTexture()->getSize();
        float scale = 180.f / std::max(ts.x, ts.y);
        c.sprite.setScale(scale, scale);
        c.sprite.setPosition(x + (cardW - (ts.x * scale)) / 2.f, y + 20.f);

        // Nom du perso
        c.nameText.setFont(font);
        c.nameText.setString(name);
        c.nameText.setCharacterSize(18);
        c.nameText.setFillColor(sf::Color::White);
        
        sf::FloatRect b = c.nameText.getLocalBounds();
        c.nameText.setOrigin(b.left + b.width/2.f, b.top);
        c.nameText.setPosition(x + cardW/2.f, y + 230.f);

        // LABEL PROPRIÉTAIRE (Vide au début)
        c.ownerText.setFont(font);
        c.ownerText.setString(""); 
        c.ownerText.setCharacterSize(60); 
        c.ownerText.setStyle(sf::Text::Bold);
        
        cards.push_back(c);
    }
}
// --- CENTRER UN TEXTE ---
void CharacterSelectionState::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(x, y);
}
// --- GESTION DES ÉVÉNEMENTS ---
void CharacterSelectionState::handleInput(GameController& game, sf::Event& event) {
    // --- SOURIS (Survol) ---
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);
        for (size_t i = 0; i < cards.size(); ++i) {
            if (cards[i].shape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                hoveredIndex = i;
            }
        }
    }

    // --- CLAVIER (Navigation) ---
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

    // --- VALIDATION (Choix du perso) ---
    bool validate = false;
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);
        if (cards[hoveredIndex].shape.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) validate = true;
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) validate = true;

    if (validate) {
        // Si déjà pris, on sort
        if (cards[hoveredIndex].isSelected) return; 

        Character chosen = availableCharacters[cards[hoveredIndex].charIndex];
        
        // --- FEEDBACK VISUEL : On affiche qui a rejoint qui ---
        std::string msg = "> " + chosen.getName() + " rejoint l'equipe J" + (currentPlayerTurn == 1 ? "1" : "2") + " ! <";
        feedbackText.setString(msg);
        centerText(feedbackText, game.window.getSize().x / 2.f, game.window.getSize().y - 50.f);

        // --- LOGIQUE DE SÉLECTION ---
        if (currentPlayerTurn == 1) {
            // Ajout au vecteur J1
            p1Selection.push_back(chosen);
            
            // Mise à jour de la carte (J1)
            cards[hoveredIndex].isSelected = true; 
            cards[hoveredIndex].ownerId = 1;
            
            cards[hoveredIndex].ownerText.setString("J1");
            cards[hoveredIndex].ownerText.setFillColor(sf::Color::Cyan);
            cards[hoveredIndex].ownerText.setOutlineColor(sf::Color::Black);
            cards[hoveredIndex].ownerText.setOutlineThickness(3.f);
            
            sf::FloatRect b = cards[hoveredIndex].ownerText.getLocalBounds();
            sf::Vector2f cPos = cards[hoveredIndex].shape.getPosition();
            sf::Vector2f cSize = cards[hoveredIndex].shape.getSize();
            cards[hoveredIndex].ownerText.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
            cards[hoveredIndex].ownerText.setPosition(cPos.x + cSize.x/2.f, cPos.y + cSize.y/2.f);

            // Transition vers J2 si 3 persos choisis
            if (p1Selection.size() == 3) {
                currentPlayerTurn = 2;
                instructionText.setString("JOUEUR 2 : Choisissez 3 champions");
                instructionText.setFillColor(sf::Color::Red);
                centerText(instructionText, game.window.getSize().x / 2.f, 120.f);
                
                // Petit message pour dire que c'est à J2
                feedbackText.setString("C'est au tour du Joueur 2 !");
                centerText(feedbackText, game.window.getSize().x / 2.f, game.window.getSize().y - 50.f);
            }
        } else {
            // Ajout au vecteur J2
            p2Selection.push_back(chosen);
            
            // Mise à jour de la carte (J2)
            cards[hoveredIndex].isSelected = true;
            cards[hoveredIndex].ownerId = 2;

            cards[hoveredIndex].ownerText.setString("J2");
            cards[hoveredIndex].ownerText.setFillColor(sf::Color::Red);
            cards[hoveredIndex].ownerText.setOutlineColor(sf::Color::Black);
            cards[hoveredIndex].ownerText.setOutlineThickness(3.f);

            sf::FloatRect b = cards[hoveredIndex].ownerText.getLocalBounds();
            sf::Vector2f cPos = cards[hoveredIndex].shape.getPosition();
            sf::Vector2f cSize = cards[hoveredIndex].shape.getSize();
            cards[hoveredIndex].ownerText.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
            cards[hoveredIndex].ownerText.setPosition(cPos.x + cSize.x/2.f, cPos.y + cSize.y/2.f);

            // LANCEMENT DU COMBAT si 3 persos choisis par J2
            if (p2Selection.size() == 3) {
                std::cout << "Lancement du combat !" << std::endl;
                game.changeState(std::make_unique<BattleState>(game, p1Selection, p2Selection));
            }
        }
    }
}

void CharacterSelectionState::update(GameController& game) {
    for (size_t i = 0; i < cards.size(); ++i) {
        bool isHovered = (i == hoveredIndex);
        
        if (cards[i].isSelected) {
            // Bordure Cyan (J1) ou Rouge (J2)
            if (cards[i].ownerId == 1) cards[i].shape.setOutlineColor(sf::Color::Cyan);
            else if (cards[i].ownerId == 2) cards[i].shape.setOutlineColor(sf::Color::Red);
            
            cards[i].shape.setOutlineThickness(4.f);
            cards[i].shape.setFillColor(sf::Color(20, 20, 20, 230)); 
            cards[i].shape.setScale(0.95f, 0.95f);
        }
        else if (isHovered) {
            // Survol (Jaune)
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

    // On dessine le texte de feedback en tout dernier
    window.draw(feedbackText);
}