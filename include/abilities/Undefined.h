#ifndef UNDEFINED_H
#define UNDEFINED_H

#include "IAbility.h"
#include "../models/Character.h"
#include <string>

class Undefined : public IAbility {
public:
    std::string getName() const override { return "undefined"; }
    int getCost() const override { return 2; }
    std::string getDescription() const override { return "An error that hurts."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(8);
    }
};

#endif