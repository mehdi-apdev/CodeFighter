#ifndef PIPE_H
#define PIPE_H

#include "IAbility.h"
#include "../models/Character.h"
#include <string>

class Pipe : public IAbility {
public:
    std::string getName() const override { return "Pipe"; }
    int getCost() const override { return 4; }
    std::string getDescription() const override { return "Redirects a stream of pain to the enemy."; }

    void execute(Character& user, Character& target) override {
        target.takeDamage(10);
    }
};

#endif