#ifndef SKILL_H
#define SKILL_H

#include <string>

class Skill {
public:
    // Attributs publics (selon le diagramme)
    int id;
    std::string name;
    std::string type;
    int staminaCost;
    std::string description;
    int damage;

    // Constructeur
    Skill(int id, const std::string& name, const std::string& type, int staminaCost, const std::string& description, int damage);

    // Méthode
    bool isUsable(int currentStamina) const;

    // Fonction d'affichage utilitaire
    void printSkill() const;
};

#endif // SKILL_H
