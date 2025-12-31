#include "../../include/models/Character.h"
#include <iostream>
#include <algorithm> // For std::find_if if searching by reference, but a simple loop is used here

// Constructor implementation
Character::Character(int id, const std::string& name, int health, const std::string& description)
    : id(id), name(name), health(health), maxHealth(health), description(description), currentStamina(100) {}


// takeDamage implementation
void Character::takeDamage(int amount) {
    // Apply damage reduction from status effects
    applyDamageReduction(amount); // amount will be modified by reference

    health -= amount;
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

// Implementations for Status Effect Management

void Character::addStatus(StatusEffect effect) {
    // Check if an effect of the same type already exists
    for (auto& s_effect : statusEffects) {
        if (s_effect.type == effect.type) {
            // Reset duration if effect exists
            s_effect.duration = effect.duration;
            s_effect.value = effect.value; // Update value as well
            std::cout << " > [Status] " << name << " refreshed " << effect.getName() << " for " << effect.duration << " turns." << std::endl;
            return;
        }
    }
    // Add new effect
    statusEffects.push_back(effect);
    std::cout << " > [Status] " << name << " gained " << effect.getName() << " for " << effect.duration << " turns." << std::endl;
}

void Character::updateStatus() {
    if (statusEffects.empty()) {
        return;
    }

    std::cout << " > [Status] " << name << "'s turn. Updating status effects." << std::endl;

    // Apply effects and decrement durations
    for (auto& effect : statusEffects) {
        switch (effect.type) {
            case StatusType::POISON:
                // Poison deals damage directly, ignoring damage reduction
                health -= effect.value;
                std::cout << " > [Status] " << name << " suffers " << effect.value << " poison damage. Health: " << health << std::endl;
                break;
            // Other effects might apply here or on interaction
            default:
                break;
        }
        effect.duration--;
    }

    // Remove expired effects
    statusEffects.erase(
        std::remove_if(statusEffects.begin(), statusEffects.end(),
                       [](const StatusEffect& effect) {
                           return effect.duration <= 0;
                       }),
        statusEffects.end());

    if (!statusEffects.empty()) {
        std::cout << " > [Status] " << name << " current active effects: ";
        for (const auto& effect : statusEffects) {
            std::cout << effect.getName() << " (" << effect.duration << "t) ";
        }
        std::cout << std::endl;
    } else {
        std::cout << " > [Status] " << name << " has no active effects." << std::endl;
    }
}

void Character::applyDamageReduction(int& incomingDamage) {
    for (const auto& effect : statusEffects) {
        if (effect.type == StatusType::BLOCK) {
            int reduction = effect.value;
            incomingDamage -= reduction;
            if (incomingDamage < 0) {
                incomingDamage = 0;
            }
            std::cout << " > [Status] " << name << " blocked " << reduction << " damage. Incoming damage reduced to " << incomingDamage << std::endl;
            break;
        }
    }
}

bool Character::isStunned() {
    for (const auto& effect : statusEffects) {
        if (effect.type == StatusType::STUN) {
            std::cout << " > [Status] " << name << " is stunned and cannot act!" << std::endl;
            return true;
        }
    }
    return false;
}
