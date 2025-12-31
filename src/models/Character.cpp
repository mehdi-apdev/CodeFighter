#include "../../include/models/Character.h"
#include <iostream>
#include <algorithm> // For std::find_if if searching by reference, but a simple loop is used here

// Constructor implementation
Character::Character(int id, const std::string& name, int health, const std::string& description)
    : id(id), name(name), health(health), maxHealth(health), description(description), currentStamina(100) {}


// takeDmg implementation
void Character::takeDmg(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

// Utility functions implementation
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
