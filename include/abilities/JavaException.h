#ifndef JAVA_EXCEPTION_H
#define JAVA_EXCEPTION_H

#include "IAbility.h"
#include "models/Character.h"
#include <iostream>
#include <string>

class JavaException : public IAbility {
public:
    void execute(Character& source, Character& target) override {
        std::cout << " > [Skill] " << source.getName() << " lance une NullPointerException inattendue !" << std::endl;
        // Specific logic: High damage
        target.takeDmg(30);
    }

    std::string getName() const override { return "Fatal Error"; }
    int getCost() const override { return 8; } // Costly
    std::string getDescription() const override { return "Crashes the opponent's system. 30 Dmg."; }
};

#endif