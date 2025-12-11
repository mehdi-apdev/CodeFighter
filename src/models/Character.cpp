#include "Character.h"
#include <iostream>
#include <algorithm> // Pour std::find_if si on cherchait par référence, mais on utilise ici une boucle simple

// Implémentation du Constructeur
Character::Character(int id, const std::string& name, int health, const std::string& description, const std::vector<Skill>& skills)
    : id(id), name(name), health(health), maxHealth(health), description(description), skills(skills), currentStamina(100) {}

// Implémentation de useSkill
bool Character::useSkill(Skill& skill, Character& targetCharacter) {
    // 1. Vérifier si le personnage a la compétence
    bool skillFound = false;
    for (const auto& s : skills) {
        if (s.name == skill.name) {
            skillFound = true;
            break;
        }
    }
    if (!skillFound) {
        std::cout << "" << name << " n'a pas la compétence " << skill.name << "!" << std::endl;
        return false;
    }

    // 2. Vérifier si la compétence est utilisable
    if (!skill.isUsable(currentStamina)) {
        std::cout << "" << name << " n'a pas assez d'endurance pour utiliser " << skill.name
                  << " (coût: " << skill.staminaCost << ", actuel: " << currentStamina << ")" << std::endl;
        return false;
    }

    // 3. Utilisation de la compétence
    currentStamina -= skill.staminaCost;
    targetCharacter.takeDmg(skill.damage);

    std::cout << "" << name << " utilise " << skill.name << " sur " << targetCharacter.name << "!" << std::endl;
    std::cout << "" << targetCharacter.name << " subit " << skill.damage << " points de dégâts." << std::endl;
    std::cout << "" << name << " perd " << skill.staminaCost << " d'endurance (reste: " << currentStamina << ")." << std::endl;

    return true;
}

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
