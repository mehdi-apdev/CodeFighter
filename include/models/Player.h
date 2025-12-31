#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "../abilities/IAbility.h"
#include "Character.h"

class Player {
private:
    std::vector<IAbility*> deck;           // Ability card deck
    std::vector<IAbility*> hand;           // Hand of 7 cards
    std::vector<Character> characters;     // List of 3 characters
    int activeCharacterIndex = 0;          // <--- ADDITION: Index of the active character on the field
    int endurance = 30;                    // Stamina (Remains for compatibility, but defeat is based on characters)

public:
    // Constructor
    Player();

    // Destructor
    ~Player();

    // Methods to manage the deck
    void addToDeck(IAbility* ability);
    void shuffleDeck();
    IAbility* drawCard();

    // Methods to manage the hand
    void addToHand(IAbility* card);
    void playCard(int index);
    void discardCard(int index);

    // --- CHARACTER MANAGEMENT (TAG TEAM) ---
    void addCharacter(const Character& character);
    Character& getCharacter(int index);
    const std::vector<Character>& getCharacters() const;

    // <--- NEW METHODS ADDED ---
    Character& getActiveCharacter();    // Returns the current fighter
    bool switchToNextCharacter();       // Switches to the next one. Returns false if all are dead.
    // ------------------------------------

    // Methods to manage stamina
    void setEndurance(int value);
    int getEndurance() const;
    void modifyEndurance(int amount);

    // Getters
    const std::vector<IAbility*>& getDeck() const;
    const std::vector<IAbility*>& getHand() const;
    size_t getHandSize() const;
    size_t getDeckSize() const;

    // Utility methods
    void initializeHand();  // Draws 7 cards at the start
    bool isDefeated() const; // Checks if all characters are dead
};

#endif // PLAYER_H