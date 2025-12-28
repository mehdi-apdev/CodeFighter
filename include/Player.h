#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "IAbility.h"
#include "Character.h"

class Player {
private:
    std::vector<IAbility*> deck;           // Deck de cartes capacité
    std::vector<IAbility*> hand;           // Main de 7 cartes
    std::vector<Character> characters;     // Liste des 3 personnages
    int activeCharacterIndex = 0;          // <--- AJOUT : Indice du personnage actif sur le terrain
    int endurance = 30;                    // Endurance (Reste là pour compatibilité, mais la défaite se joue aux persos)

public:
    // Constructeur
    Player();

    // Destructeur
    ~Player();

    // Méthodes pour gérer le deck
    void addToDeck(IAbility* ability);
    void shuffleDeck();
    IAbility* drawCard();

    // Méthodes pour gérer la main
    void addToHand(IAbility* card);
    void playCard(int index);
    void discardCard(int index);

    // --- GESTION DES PERSONNAGES (TAG TEAM) ---
    void addCharacter(const Character& character);
    Character& getCharacter(int index);
    const std::vector<Character>& getCharacters() const;

    // <--- NOUVELLES MÉTHODES AJOUTÉES ---
    Character& getActiveCharacter();    // Renvoie le combattant actuel
    bool switchToNextCharacter();       // Passe au suivant. Renvoie false si tout le monde est mort.
    // ------------------------------------

    // Méthodes pour gérer l'endurance
    void setEndurance(int value);
    int getEndurance() const;
    void modifyEndurance(int amount);

    // Getters
    const std::vector<IAbility*>& getDeck() const;
    const std::vector<IAbility*>& getHand() const;
    size_t getHandSize() const;
    size_t getDeckSize() const;

    // Méthodes utilitaires
    void initializeHand();  // Pioche 7 cartes au début
    bool isDefeated() const; // Vérifie si tous les personnages sont morts
};

#endif // PLAYER_H