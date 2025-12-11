#include "Player.h"
#include <algorithm>
#include <random>

// Constructeur
Player::Player() : endurance(30) {
    // La main sera initialisée avec initializeHand()
    characters.reserve(3); // Réserve de l'espace pour 3 personnages
}

// Destructeur
Player::~Player() {
    // Libération de la mémoire des cartes du deck
    for (IAbility* card : deck) {
        delete card;
    }

    // Libération de la mémoire des cartes de la main
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
        // Ici vous devrez implémenter la logique pour jouer la carte
        // Pour l'instant on retire simplement la carte de la main
        hand.erase(hand.begin() + index);
    }
}

void Player::discardCard(int index) {
    if (index >= 0 && index < static_cast<int>(hand.size())) {
        delete hand[index]; // Libère la mémoire
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
    if (endurance < 0) endurance = 0; // Évite les valeurs négatives
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
    return endurance <= 0;
}
