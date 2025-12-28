#ifndef JAVA_EXCEPTION_H
#define JAVA_EXCEPTION_H

#include "../IAbility.h"
#include "../Character.h"
#include <iostream>
#include <string>

class JavaException : public IAbility {
public:
    void execute(Character& source, Character& target) override {
        std::cout << " > [Skill] " << source.getName() << " lance une NullPointerException inattendue !" << std::endl;
        // Logique spécifique : Gros dégâts
        target.takeDmg(30);
    }

    std::string getName() const override { return "Fatal Error"; }
    int getCost() const override { return 8; } // Coûteux
    std::string getDescription() const override { return "Crash le systeme adverse. 30 Dmg."; }
};

#endif