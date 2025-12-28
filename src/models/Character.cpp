#include "../../include/Character.h"
#include <iostream>
#include <algorithm> // Pour std::find_if si on cherchait par référence, mais on utilise ici une boucle simple

// Implémentation du Constructeur
Character::Character(int id, const std::string& name, int health, const std::string& description)
    : id(id), name(name), health(health), maxHealth(health), description(description), currentStamina(100) {}


// Implémentation de takeDmg
void Character::takeDmg(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

// Implémentation des fonctions utilitaires
bool Character::isAlive() const {
    return health > 0;
}

void Character::printStatus() const {
    std::string status = isAlive() ? "Vivant" : "KO";
    std::cout << "" << name << " | Vie: " << health << "/" << maxHealth
              << " | Endurance: " << currentStamina << " | Statut: " << status << std::endl;
}

const std::string& Character::getName() const {
    return name;
}
