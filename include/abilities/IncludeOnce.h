#ifndef INCLUDEONCE_H
#define INCLUDEONCE_H

#include "IAbility.h"
#include "../models/Character.h"
#include <string>

class IncludeOnce : public IAbility {
public:
    std::string getName() const override { return "include_once"; }
    int getCost() const override { return 4; }
    std::string getDescription() const override { return "Forces the inclusion of external damage."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(10);
    }
};

#endif