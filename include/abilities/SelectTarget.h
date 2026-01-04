#ifndef SELECTTARGET_H
#define SELECTTARGET_H

#include "IAbility.h"
#include "../models/Character.h"
#include <string>

class SelectTarget : public IAbility {
public:
    std::string getName() const override { return "Select * from Target"; }
    int getCost() const override { return 5; }
    std::string getDescription() const override { return "Selects a target for your next ability."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(15);
    }
};
    



#endif