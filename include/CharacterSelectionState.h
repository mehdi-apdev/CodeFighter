#ifndef CHARACTER_SELECTION_STATE_H 
#define CHARACTER_SELECTION_STATE_H

#include "State.h"
#include "Character.h"
#include <SFML/Graphics.hpp>
#include <vector>

class GameController; 

class CharacterSelectionState : public State
{
private:
    sf::Font& font;
    
    // --- GESTION DU FOND ---
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // --- DONNÉES DE JEU ---
    std::vector<Character> availableCharacters; 
    std::vector<Character> p1Selection; 
    std::vector<Character> p2Selection; 
    int currentPlayerTurn = 1; 

    // --- UI & ASSETS ---
    sf::Texture pyraTexture;
    sf::Texture javaTexture;

    sf::Text titleText;
    sf::Text instructionText;
    
  
    sf::Text feedbackText; 


    // Structure interne pour une "Carte" de personnage
    struct CharCard {
        sf::RectangleShape shape;
        sf::Sprite sprite;
        sf::Text nameText;
        sf::Text ownerText; // Affiche "J1" ou "J2"
        int charIndex;      
        bool isSelected;    
        int ownerId; // 0=Personne, 1=J1, 2=J2
    };
    std::vector<CharCard> cards;

    int hoveredIndex = 0; 

    void initRoster();
    void loadAssets(float width, float height); 
    void initUI(float width, float height); 
    void centerText(sf::Text& text, float x, float y);

public:
    CharacterSelectionState(GameController& game);

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;
};

#endif