#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "../models/Player.h"
#include "../views/CharacterView.h"
#include "../views/CardView.h"
#include "../abilities/AllAbilities.h"
#include <vector>  // <--- ESSENTIAL for std::vector
#include <memory>  // <--- ESSENTIAL for std::unique_ptr

class BattleState : public State {
public:
    // Internal enum for battle flow
    enum class BattlePhase { InterTurn, WaitingForInput, TurnTransition, CombatEnd };

private:
    // Match data
    Player player1;
    Player player2;
    Player* activePlayer = nullptr;
    Player* inactivePlayer = nullptr;

    // sf::Texture backgroundTexture; // <-- Removed
    sf::Sprite backgroundSprite;

    std::unique_ptr<CharacterView> pyraView;
    std::unique_ptr<CharacterView> javaTronView;
    std::vector<std::unique_ptr<CardView>> handViews;
    CardView* selectedCard = nullptr;

    sf::Text promptText;
    sf::Text infoText;
    BattlePhase currentPhase;

    // Private internal methods
    void initMatch();
    void updateHandViews();
    void centerText(sf::Text& text, float x, float y);

public:
    // --- CRITICAL CORRECTION HERE ---
    // We declare that the constructor accepts teams (std::vector)
    BattleState(GameController& game, std::vector<Character> p1Team, std::vector<Character> p2Team);

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;
};

#endif