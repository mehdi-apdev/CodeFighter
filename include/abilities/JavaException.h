#ifndef JAVAEXCEPTION_H
#define JAVAEXCEPTION_H

#include "IAbility.h"
#include <string>

class JavaException : public IAbility {
public:
    // Getters
    std::string getName() const override { return "Java Exception"; }
    int getCost() const override { return 20; }
    std::string getDescription() const override { return "Crashes the opponent's system. 30 Dmg."; }

    // Logique
    void execute(Character& user, Character& target) override {
        target.takeDamage(30);
    }
};

#endif
