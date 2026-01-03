#ifndef GOROUTINE_H
#define GOROUTINE_H

#include "IAbility.h"
#include "../models/Character.h"
#include <string>

class Goroutine : public IAbility {
public:
    std::string getName() const override { return "Goroutine"; }
    int getCost() const override { return 3; }
    std::string getDescription() const override { return "Launches concurrent attack processes."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(8);
    }
};

#endif