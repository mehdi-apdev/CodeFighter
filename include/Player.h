#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "IAbility.h"
#include "Character.h"

class Player {
private:

    std::vector<IAbility*> deck;           
    std::vector<IAbility*> hand;           
    std::vector<IAbility*> discardPile;    
    std::vector<Character> characters;     
    int endurance = 30;
    
    bool reshuffleTriggered = false;

public:
    Player();
    ~Player();

    // Deck/Hand Methods
    void addToDeck(IAbility* ability);
    void shuffleDeck();
    IAbility* drawCard();
    void addToHand(IAbility* card);
    void playCard(int index);
    void discardCard(int index);


    // NEW: Refills the hand up to a certain size
    void refillHand(size_t targetSize);

    // Characters
    void addCharacter(const Character& character);
    Character& getCharacter(int index);
    const std::vector<Character>& getCharacters() const;
    Character* getActiveCharacter();

    // Endurance

    void setEndurance(int value);
    int getEndurance() const;
    void modifyEndurance(int amount);

    // Getters
    const std::vector<IAbility*>& getDeck() const;
    const std::vector<IAbility*>& getHand() const;
    size_t getHandSize() const;
    size_t getDeckSize() const;

    size_t getDiscardSize() const { return discardPile.size(); }

    void initializeHand();  
    bool isDefeated() const;
    
    bool hasReshuffled() const { return reshuffleTriggered; }
    void clearReshuffleFlag() { reshuffleTriggered = false; }

};

#endif // PLAYER_H