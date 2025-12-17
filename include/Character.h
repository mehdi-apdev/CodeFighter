#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include "IAbility.h"

class Character {
private:
    // Attributs
    int id;
    std::string name;
    int health;
    int maxHealth; // Ajout pour le suivi
    std::string description;
    int currentStamina; // Ajout pour la démo

public:
    // Constructeur
    Character(int id, const std::string& name, int health, const std::string& description);

    // Méthodes
    void useAbility();
    void takeDmg(int damage);

    // Fonctions utilitaires
    bool isAlive() const;
    void printStatus() const;
    const std::string& getName() const; // Getter simple
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
};

#endif // CHARACTER_H
