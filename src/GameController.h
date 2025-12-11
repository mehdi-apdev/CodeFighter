#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Character.h"
#include "CharacterView.h"
#include "Skill.h"

class GameController {
public:
    // Renommé en GameState pour inclure MainMenu
    enum class GameState {
        MainMenu,
        WaitingForInput, // Attente de l'action du joueur (Pyra)
        PlayerTurn,      // Le joueur agit
        EnemyTurn,       // L'ennemi (Java-Tron) agit
        TurnTransition,  // Vérifie l'état du combat et passe au tour suivant
        CombatEnd        // Combat terminé
    };

    // Fenêtre de rendu
    sf::RenderWindow window;

    // Ressources partagées
    sf::Font font;

    // Models (Logique du jeu)
    Character pyraModel;
    Character javaTronModel;

    // Skills
    Skill coupPyra;
    Skill exceptionJava;

    // Views (Affichage)
    std::unique_ptr<CharacterView> pyraView;
    std::unique_ptr<CharacterView> javaTronView;

    // UI Text
    sf::Text promptText;
    sf::Text infoText;

    // Variables de contrôle du tour
    GameState currentState; // CORRIGÉ : utilise GameState
    Character* activeCharacter = nullptr; // Personnage dont c'est le tour
    Character* inactiveCharacter = nullptr; // Personnage qui est ciblé

    // Fonctions privées pour la gestion interne
    void initResources();
    void initModelsAndViews();
    void handleEvents();
    void updateGameStatus();
    void processEnemyTurn();
    void render();

public:
    // Constructeur et Destructeur
    GameController();
    ~GameController() = default;

    // Méthode principale pour démarrer le jeu
    void run();
};

#endif // GAMECONTROLLER_H
