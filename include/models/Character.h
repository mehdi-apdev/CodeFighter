#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include "../abilities/IAbility.h"

class Character {
private:
    // Attributes
    int id;
    std::string name;
    int health;
    int maxHealth; // Added for tracking
    std::string description;
    int currentStamina; // Added for demo

public:
    // Constructor
    Character(int id, const std::string& name, int health, const std::string& description);

    // Methods
    void useAbility();
    void takeDmg(int damage);

    // Utility functions
    bool isAlive() const;
    void printStatus() const;
    const std::string& getName() const; // Simple Getter
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
};

#endif // CHARACTER_H
