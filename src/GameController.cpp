#include "GameController.h"
#include <iostream>
#include <sstream>

// Renommer l'énumération pour une portée plus large (pour la fonction toString)
using GameState = GameController::GameState;

// Fonction utilitaire pour la conversion
template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// Constructeur
GameController::GameController()
    : window(sf::VideoMode(900, 700), "SFML Combat MVC"),
      // Initialisation des Models et Skills (valeurs d'initialisation utilisées, les vraies valeurs sont dans initModelsAndViews)
      pyraModel(0, "Pyra (Python)", 100, "", {}),
      javaTronModel(0, "Java-Tron", 80, "", {}),
      coupPyra(1, "Attaque Pyra", "Code Python", 10, "Attaque de base rapide.", 20),
      exceptionJava(3, "Exception Java", "Dégâts purs", 25, "L'erreur fatale Java.", 45),
      currentState(GameState::WaitingForInput)
{
    initResources();
    initModelsAndViews(); // La première exécution configure le menu et les modèles
}

// Chargement des ressources partagées (Font, etc.)
void GameController::initResources() {
    // UTILISATION DE CHEMINS RELATIFS pour la portabilité (supprimer les chemins C:/...)
    if (!font.loadFromFile("ARIAL.TTF")) {
        std::cerr << "Erreur lors du chargement de la police 'ARIAL.TTF'. Assurez-vous que le fichier est présent." << std::endl;
        // En production, on pourrait fermer la fenêtre ici ou utiliser une police par défaut.
    }
}

// Initialisation/Réinitialisation des Models et de leurs Views associées
void GameController::initModelsAndViews() {
    // Re-configuration des Models (PV remis à jour pour un nouveau match)
    pyraModel = Character(101, "Pyra (Python)", 100, "Le code dynamique.", {coupPyra});
    javaTronModel = Character(102, "Java-Tron", 80, "La machine virtuelle.", {exceptionJava});

    // Définir Pyra comme le premier joueur actif
    activeCharacter = &pyraModel;
    inactiveCharacter = &javaTronModel;

    float imageScale = 0.4f;

    // Création des Views (utilisation de chemins relatifs)
    pyraView = std::make_unique<CharacterView>(pyraModel, "pyra_python.png", font, 50.f, 60.f, imageScale);
    javaTronView = std::make_unique<CharacterView>(javaTronModel, "java_tron.png", font, 480.f, 60.f, imageScale);

    // Initialisation du Texte de l'Interface de Combat
    promptText.setFont(font);
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(50, 580); // Ajusté à 580

    infoText.setFont(font);
    infoText.setCharacterSize(18);
    infoText.setFillColor(sf::Color::Cyan);
    infoText.setPosition(50, 650);
    infoText.setString("C'est le tour de Pyra (Python). Appuyez sur '1' pour attaquer.");
}

// Gestion des événements (entrées utilisateur)
void GameController::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }
        if (currentState == GameState::WaitingForInput) {
            if (event.type == sf::Event::KeyPressed) {
                if (activeCharacter == &pyraModel) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        // Pyra attaque, puis passe à la vérification
                        pyraModel.useSkill(coupPyra, javaTronModel);
                        currentState = GameState::TurnTransition;
                    }
                }
            }
        }
    }
}

// Logique du tour de l'IA (Java-Tron)
void GameController::processEnemyTurn() {
    if (activeCharacter == &javaTronModel && javaTronModel.isAlive()) {
        std::cout << "--- Tour de l'IA ---" << std::endl;
        javaTronModel.useSkill(exceptionJava, pyraModel);

        // L'action de l'IA est terminée, on passe à la vérification
        currentState = GameState::TurnTransition;
    } else {
        currentState = GameState::CombatEnd;
    }
}

// Mise à jour de l'état du jeu et séquence des tours
void GameController::updateGameStatus() {
    // Cette logique ne s'exécute que si le combat est actif
    if (currentState == GameState::MainMenu || currentState == GameState::CombatEnd) {
        return;
    }

    // 1. Vérification de la fin du combat
    if (!pyraModel.isAlive() || !javaTronModel.isAlive()) {
        currentState = GameState::CombatEnd;
    }

    // 2. Logique de l'IA (EnemyTurn)
    if (currentState == GameState::EnemyTurn) {
        processEnemyTurn();
    }

    // 3. Gérer la transition après une action (Player ou Enemy)
    if (currentState == GameState::TurnTransition) {

        // Si le combat n'est pas terminé après l'action
        if (pyraModel.isAlive() && javaTronModel.isAlive()) {

            // Inverser les rôles
            Character* temp = activeCharacter;
            activeCharacter = inactiveCharacter;
            inactiveCharacter = temp;

            // Déterminer le prochain état
            if (activeCharacter == &pyraModel) {
                currentState = GameState::WaitingForInput; // Retour au joueur
            } else {
                currentState = GameState::EnemyTurn; // Passer immédiatement au tour de l'IA
            }
        }
    }

    // 4. Mise à jour du texte de l'interface
    std::string statusMessage;

    if (currentState == GameState::CombatEnd) {
        if (!pyraModel.isAlive()) {
            statusMessage = "FIN DU COMBAT : Java-Tron gagne !";
        } else {
            statusMessage = "FIN DU COMBAT : Pyra gagne !";
        }
        infoText.setString("Appuyez sur ESC pour quitter.");
    } else {
        // Affichage de l'état des PV
        statusMessage = pyraModel.getName() + " (Vie: " + toString(pyraModel.getHealth()) + ") vs "
                      + javaTronModel.getName() + " (Vie: " + toString(javaTronModel.getHealth()) + ")";

        // Affichage de l'instruction du tour
        if (currentState == GameState::WaitingForInput) {
            infoText.setString("C'est le tour de Pyra (Python). Appuyez sur '1' pour attaquer.");
        } else if (currentState == GameState::EnemyTurn) {
             infoText.setString("Java-Tron est en train d'agir...");
        }
    }

    promptText.setString(statusMessage);
}


// Rendu : demande aux Views de dessiner
void GameController::render() {
    window.clear(sf::Color(20, 20, 40));

    // Rendu du Combat
    pyraView->draw(window);
    javaTronView->draw(window);

    window.draw(promptText);
    window.draw(infoText);

    window.display();
}

// Boucle principale du jeu
void GameController::run() {
    while (window.isOpen()) {
        handleEvents();
        updateGameStatus();
        render();
    }
}
