#ifndef PYTHONSTRIKE_H
#define PYTHONSTRIKE_H

#include "IAbility.h" 
#include <string>

class PythonStrike : public IAbility {
public:
    std::string getName() const override { return "Python Strike"; }
    int getCost() const override { return 10; }
    std::string getDescription() const override { return "Fast and flexible attack. 15 Dmg."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(15);
    }
};

#endif