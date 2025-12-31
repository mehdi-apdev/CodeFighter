#ifndef CHARACTER_SELECTION_STATE_H 
#define CHARACTER_SELECTION_STATE_H

#include "states/State.h"
#include "models/Character.h"
#include <SFML/Graphics.hpp>
#include <vector>

class GameController; 

class CharacterSelectionState : public State
{
private:
    // UI & Assets
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture pyraTexture;
    sf::Texture javaTexture;

    sf::Text titleText;
    sf::Text instructionText;
    sf::Text feedbackText; 

    // Data
    std::vector<Character> availableCharacters; 
    std::vector<Character> p1Selection; 
    std::vector<Character> p2Selection; 
    int currentPlayerTurn = 1; 

    // Internal structure for a "Card"
    struct CharCard {
        sf::RectangleShape shape;
        sf::Sprite sprite;
        sf::Text nameText;
        sf::Text ownerText;
        int charIndex;      
        bool isSelected;    
        int ownerId; // 0=None, 1=P1, 2=P2
    };
    std::vector<CharCard> cards;

    int hoveredIndex = 0; 

    void initRoster();
    void initUI(float width, float height); 
    void centerText(sf::Text& text, float x, float y);

public:
    CharacterSelectionState(GameController& game);
    virtual ~CharacterSelectionState() = default;

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;
};

#endif
