#ifndef PLAYER_H
#define PLAYER_H


#include <vector>
#include "IAbility.h"
#include "Character.h"

class Player {
private:
    std::vector<IAbility*> deck;           // Deck de cartes capacit�
    std::vector<IAbility*> hand;           // Main de 7 cartes
    std::vector<Character> characters;     // 3 personnages
    int endurance =30;                         // Endurance (30 points)

public:
    // Constructeur
    Player();

    // Destructeur
    ~Player();

    // M�thodes pour g�rer le deck
    void addToDeck(IAbility* ability);
    void shuffleDeck();
    IAbility* drawCard();

    // M�thodes pour g�rer la main
    void addToHand(IAbility* card);
    void playCard(int index);
    void discardCard(int index);

    // M�thodes pour g�rer les personnages
    void addCharacter(const Character& character);
    Character& getCharacter(int index);
    const std::vector<Character>& getCharacters() const;

    // M�thodes pour g�rer l'endurance
    void setEndurance(int value);
    int getEndurance() const;
    void modifyEndurance(int amount);

    // Getters
    const std::vector<IAbility*>& getDeck() const;
    const std::vector<IAbility*>& getHand() const;
    size_t getHandSize() const;
    size_t getDeckSize() const;

    // M�thodes utilitaires
    void initializeHand();  // Pioche 7 cartes au d�but
    bool isDefeated() const; // V�rifie si le joueur est vaincu
};

#endif // PLAYER_H
