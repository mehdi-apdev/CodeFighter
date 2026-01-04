#ifndef FLEXBOX_H
#define FLEXBOX_H

#include "IAbility.h"
#include "../models/Character.h"
#include <string>

class Flexbox : public IAbility {
public:
    std::string getName() const override { return "Flexbox"; }
    int getCost() const override { return 5; }
    std::string getDescription() const override { return "Reorganizes the opponent's structure brutally."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(12);
    }
};

#endif