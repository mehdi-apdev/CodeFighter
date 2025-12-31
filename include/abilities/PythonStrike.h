#ifndef PYTHON_STRIKE_H
#define PYTHON_STRIKE_H

#include "IAbility.h"
#include "models/Character.h"
#include <iostream>
#include <string>

class PythonStrike : public IAbility {
public:
    void execute(Character& source, Character& target) override {
        std::cout << " > [Skill] " << source.getName() << " ondule comme un serpent et frappe !" << std::endl;
        // Specific logic: Moderate damage
        target.takeDmg(15);
    }

    std::string getName() const override { return "Python Strike"; }
    int getCost() const override { return 3; } // Inexpensive
    std::string getDescription() const override { return "Fast and flexible attack. 15 Dmg."; }
};

#endif