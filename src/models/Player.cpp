#include "../../include/models/Player.h"
#include <algorithm>
#include <random>
#include <stdexcept> // Required to handle errors if the list is empty

// Constructeur
// MODIFICATION: Initialization of the active character index to 0
Player::Player() : endurance(30), activeCharacterIndex(0) {
    // The hand will be initialized with initializeHand()
    characters.reserve(3); // Reserve space for 3 characters
}

// Destructeur
Player::~Player() {
    // Freeing memory of deck cards
    for (IAbility* card : deck) {
        delete card;
    }

    // Freeing memory of hand cards
    for (IAbility* card : hand) {
        delete card;
    }
}

// Méthodes pour gérer le deck
void Player::addToDeck(IAbility* ability) {
    deck.push_back(ability);
}

void Player::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

IAbility* Player::drawCard() {
    if (deck.empty()) {
        return nullptr;
    }
    IAbility* card = deck.back();
    deck.pop_back();
    return card;
}

// Méthodes pour gérer la main
void Player::addToHand(IAbility* card) {
    if (card != nullptr) {
        hand.push_back(card);
    }
}

void Player::playCard(int index) {
    if (index >= 0 && index < static_cast<int>(hand.size())) {
        // Here you will need to implement the logic to play the card
        // For now, we simply remove the card from the hand
        hand.erase(hand.begin() + index);
    }
}

void Player::discardCard(int index) {
    if (index >= 0 && index < static_cast<int>(hand.size())) {
        delete hand[index]; // Frees memory
        hand.erase(hand.begin() + index);
    }
}

// Méthodes pour gérer les personnages
void Player::addCharacter(const Character& character) {
    if (characters.size() < 3) {
        characters.push_back(character);
    }
}

Character& Player::getCharacter(int index) {
    return characters[index];
}

// --- AJOUTS POUR LE SYSTÈME DE COMBAT (TAG TEAM) ---

Character& Player::getActiveCharacter() {
    if (characters.empty()) {
        // Safety: avoids a crash if called without characters
        throw std::runtime_error("Erreur : Aucun personnage dans l'equipe !");
    }
    
    // Additional safety: if the index exceeds, returns the last one
    if (activeCharacterIndex >= static_cast<int>(characters.size())) {
        return characters.back();
    }
    
    return characters[activeCharacterIndex];
}

bool Player::switchToNextCharacter() {
    // Vérifie s'il reste un personnage APRÈS celui-ci dans la liste
    if (activeCharacterIndex < static_cast<int>(characters.size()) - 1) {
        activeCharacterIndex++;
        return true; // Le changement a réussi, un nouveau perso arrive
    }
    return false; // Failure: It was the last character, the player lost
}

// ---------------------------------------------------

const std::vector<Character>& Player::getCharacters() const {
    return characters;
}

// Méthodes pour gérer l'endurance
void Player::setEndurance(int value) {
    endurance = value;
}

int Player::getEndurance() const {
    return endurance;
}

void Player::modifyEndurance(int amount) {
    endurance += amount;
    if (endurance < 0) endurance = 0; // Avoids negative values
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

// Méthodes utilitaires
void Player::initializeHand() {
    for (int i = 0; i < 7 && !deck.empty(); ++i) {
        IAbility* card = drawCard();
        if (card != nullptr) {
            addToHand(card);
        }
    }
}

bool Player::isDefeated() const {
    // MODIFICATION: The player is defeated if their team is empty or if the last character is dead
    
    if (characters.empty()) return true;

    // If on the last character of the list...
    bool isLastCharacter = (activeCharacterIndex >= static_cast<int>(characters.size()) - 1);
    
    // ...and it has no more health (assuming getHealth() exists via Character.h)
    // Note: We access the last element directly to be sure
    bool isDead = (characters.back().getHealth() <= 0);

    return isLastCharacter && isDead;
}