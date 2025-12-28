#include "../../include/Player.h"
#include <algorithm>
#include <random>


// Constructor
Player::Player() : endurance(30) {
    characters.reserve(3); 
}

// Destructor
Player::~Player() {
    for (IAbility* card : deck) delete card;
    for (IAbility* card : hand) delete card;
    for (IAbility* card : discardPile) delete card;
}

// Methods to manage the deck
void Player::addToDeck(IAbility* ability) {
    deck.push_back(ability);
}

void Player::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}


// MODIFIED: New strict recycling logic
IAbility* Player::drawCard() {
    // 1. If the deck is empty
    if (deck.empty()) {
        
        // --- NEW CONDITION ---
        // If the hand is NOT empty, forbid recycling.
        // Return nullptr, which will stop drawing (refillHand will stop there).
        if (!hand.empty()) {
            return nullptr;
        }

        // If the hand is empty AND the discard pile is empty, it's over (no cards left at all)
        if (discardPile.empty()) {
            return nullptr;
        }

        // 2. Otherwise (Empty hand + Empty deck + Full discard), recycle!
        deck = discardPile;
        discardPile.clear();

        // 3. Shuffle
        shuffleDeck();
        
        // 4. Signal the event for animation
        reshuffleTriggered = true;
    }

    // 5. Draw
    IAbility* card = deck.back();
    deck.pop_back();
    return card;
}


// Methods to manage the hand
void Player::addToHand(IAbility* card) {
    if (card != nullptr) {
        hand.push_back(card);
    }
}

void Player::playCard(int index) {
    if (index >= 0 && index < static_cast<int>(hand.size())) {

        // The card goes to the discard pile
        discardPile.push_back(hand[index]);
        hand.erase(hand.begin() + index);
    }
}

void Player::discardCard(int index) {
    if (index >= 0 && index < static_cast<int>(hand.size())) {

        discardPile.push_back(hand[index]);
        hand.erase(hand.begin() + index);
    }
}


// Refills the hand up to a certain size
void Player::refillHand(size_t targetSize) {
    while (hand.size() < targetSize) {
        IAbility* card = drawCard();
        // If drawCard returns nullptr (because the deck is empty and we still have cards in hand),
        // the loop stops. The player will start their turn with fewer cards.
        if (card != nullptr) {
            addToHand(card);
        } else {
            break; 
        }
    }
}

// Methods to manage characters
void Player::addCharacter(const Character& character) {
    if (characters.size() < 3) {
        characters.push_back(character);
    }
}

Character& Player::getCharacter(int index) {
    return characters[index];
}


const std::vector<Character>& Player::getCharacters() const {
    return characters;
}

// Méthodes pour gérer l'endurance
Character* Player::getActiveCharacter() {
    for (auto& character : characters) {
        if (character.isAlive()) {
            return &character;
        }
    }
    return nullptr; // All dead
}

// Methods to manage endurance
void Player::setEndurance(int value) {
    endurance = value;
}

int Player::getEndurance() const {
    return endurance;
}

void Player::modifyEndurance(int amount) {
    endurance += amount;

    if (endurance < 0) endurance = 0; 
}

// Getters
const std::vector<IAbility*>& Player::getDeck() const {
    return deck;
}

const std::vector<IAbility*>& Player::getHand() const {
    return hand;
}

size_t Player::getHandSize() const {
    return hand.size();
}

size_t Player::getDeckSize() const {
    return deck.size();
}


// Utility methods
void Player::initializeHand() {
    refillHand(7);
}

bool Player::isDefeated() const {
    for (const auto& character : characters) {
        if (character.isAlive()) {
            return false; 
        }
    }
    return true;
}