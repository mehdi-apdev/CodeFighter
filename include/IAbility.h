#ifndef IABILITY_H
#define IABILITY_H

#include <string> // Nécessaire pour std::string

// Forward Declaration :
// On dit au compilateur que "Character" existe, sans inclure tout le fichier
// pour éviter les boucles d'inclusions.
class Character; 

class IAbility {
public:
    // Destructeur virtuel par défaut (Très important pour une interface !)
    virtual ~IAbility() = default;

    // --- LE CŒUR DU PATTERN COMMAND ---
    // = 0 signifie "Virtuelle Pure". Les classes filles DOIVENT coder cette méthode.
    virtual void execute(Character& source, Character& target) = 0;

    // --- INFO UI (Sprint 3) ---
    // Le "const" à la fin promet qu'on ne modifiera pas l'objet en appelant ces fonctions.
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual std::string getDescription() const = 0;
};

#endif