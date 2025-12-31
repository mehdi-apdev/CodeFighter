#ifndef IABILITY_H
#define IABILITY_H

#include <string> // Required for std::string

// Forward Declaration :
// We tell the compiler that "Character" exists, without including the whole file
// to avoid inclusion loops.
class Character; 

class IAbility {
public:
    // Default virtual destructor (Very important for an interface!)
    virtual ~IAbility() = default;

    // --- THE CORE OF THE COMMAND PATTERN ---
    // = 0 means "Pure Virtual". Derived classes MUST implement this method.
    virtual void execute(Character& source, Character& target) = 0;

    // --- UI INFO (Sprint 3) ---
    // The "const" at the end guarantees that the object will not be modified when calling these functions.
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual std::string getDescription() const = 0;
};

#endif