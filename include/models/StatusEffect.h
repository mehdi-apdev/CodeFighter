#ifndef STATUS_EFFECT_H
#define STATUS_EFFECT_H

#include <string>
#include <vector> // Potentially useful for Character to include this

// Enum for different types of status effects
enum class StatusType {
    BLOCK,          // Blocks X damage or reduces by a percentage
    POISON,         // Damage per turn
    STUN,           // Skips a turn
    DAMAGE_BOOST,   // Increases damage dealt
    DEFENSE_BOOST   // Increases defense
};

// Structure to represent a single status effect applied to a character
struct StatusEffect {
    StatusType type;    // The type of status effect
    int duration;       // Number of turns remaining for the effect
    int value;          // Power of the effect (e.g., poison damage, block amount, percentage boost)

    // Utility method to get a readable name for the status type
    std::string getName() const {
        switch (type) {
            case StatusType::BLOCK:         return "Block";
            case StatusType::POISON:        return "Poison";
            case StatusType::STUN:          return "Stun";
            case StatusType::DAMAGE_BOOST:  return "Damage Boost";
            case StatusType::DEFENSE_BOOST: return "Defense Boost";
            default:                        return "Unknown Effect";
        }
    }
};

#endif // STATUS_EFFECT_H
