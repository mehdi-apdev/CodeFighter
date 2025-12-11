#include "Skill.h"
#include <iostream>

// Implémentation du Constructeur
Skill::Skill(int id, const std::string& name, const std::string& type, int staminaCost, const std::string& description, int damage)
    : id(id), name(name), type(type), staminaCost(staminaCost), description(description), damage(damage) {}

// Implémentation de isUsable
bool Skill::isUsable(int currentStamina) const {
    return currentStamina >= staminaCost;
}

// Implémentation de printSkill
void Skill::printSkill() const {
    std::cout << name << " (Type: " << type << ", Coût: " << staminaCost << " endurance, Dégâts: " << damage << ")";
}
