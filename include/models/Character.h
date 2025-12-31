#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include "../abilities/IAbility.h"
#include "StatusEffect.h"

class Character {
private:
    // Attributes
    int id;
    std::string name;
    int health;
    int maxHealth; // Added for tracking
    std::string description;
    int currentStamina; // Added for demo
    std::vector<StatusEffect> statusEffects; // List of active status effects

public:
    // Constructor
    Character(int id, const std::string& name, int health, const std::string& description);

    // Methods
    void useAbility();
    void takeDamage(int amount); // Renamed from takeDmg

    // Status Effect Management
    void addStatus(StatusEffect effect);
    void updateStatus(); // Applies effects like poison, decrements durations
    void applyDamageReduction(int& incomingDamage); // Checks for BLOCK/DEFENSE boosts
    bool isStunned(); // Checks if STUN is active
    bool isAlive() const;
    void printStatus() const;
    const std::string& getName() const; // Simple Getter
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
};

#endif // CHARACTER_H
