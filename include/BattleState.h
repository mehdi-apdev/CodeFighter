#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Player.h"
#include "CharacterView.h"
#include "CardView.h"
#include "abilities/AllAbilities.h"


#include <vector>
#include <memory>
#include <SFML/System/Clock.hpp> // For animation time

class BattleState : public State {
public:
    enum class BattlePhase { InterTurn, WaitingForInput, TurnTransition, CombatEnd };

private:

    Player player1;
    Player player2;
    Player* activePlayer = nullptr;
    Player* inactivePlayer = nullptr;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::unique_ptr<CharacterView> pyraView;
    std::unique_ptr<CharacterView> javaTronView;
    std::vector<std::unique_ptr<CardView>> handViews;


    
    CardView* selectedCard = nullptr;
    int selectedCardIndex = -1; // To know which card to play in the vector

    sf::Text promptText;
    sf::Text infoText;
    
    // Variables for the reshuffle animation
    sf::Text reshuffleText;      
    sf::Clock animClock;         
    float reshuffleDisplayTime = 0.0f; 

    BattlePhase currentPhase;
    sf::Font& font;

    void initMatch();
    void updateHandViews();
    void centerText(sf::Text& text, float x, float y);

public:


    BattleState(GameController& game); 

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;
};

#endif