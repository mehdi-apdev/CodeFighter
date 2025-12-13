// include/ConcreteAbilities.h
#ifndef CONCRETE_ABILITIES_H
#define CONCRETE_ABILITIES_H

#include "IAbility.h"
#include "Character.h"
#include <iostream>

// --- Capacité Pyra (Python) ---
class PythonStrike : public IAbility {
public:
    void execute(Character& source, Character& target) override {
        std::cout << source.getName() << " execute PythonStrike !" << std::endl;
        // Logique de dégâts
        target.takeDmg(15);
    }

    std::string getName() const override { return "Python Strike"; }
    int getCost() const override { return 3; }
    std::string getDescription() const override { return "Attaque rapide et flexible. 15 Dmg."; }
};

// --- Capacité Java-Tron ---
class JavaException : public IAbility {
public:
    void execute(Character& source, Character& target) override {
        std::cout << source.getName() << " lance une NullPointerException !" << std::endl;
        // Dégâts lourds mais coûteux
        target.takeDmg(30);
    }

    std::string getName() const override { return "Fatal Error"; }
    int getCost() const override { return 8; }
    std::string getDescription() const override { return "Crash le systeme adverse. 30 Dmg."; }
};

#endif
