#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Player.h"
#include "CharacterView.h"
#include "CardView.h"
#include "abilities/AllAbilities.h"
#include <vector>  // <--- INDISPENSABLE pour std::vector
#include <memory>  // <--- INDISPENSABLE pour std::unique_ptr

class BattleState : public State {
public:
    // Enum interne pour le flux du combat
    enum class BattlePhase { InterTurn, WaitingForInput, TurnTransition, CombatEnd };

private:
    // Données du match
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

    sf::Text promptText;
    sf::Text infoText;
    BattlePhase currentPhase;
    sf::Font& font;

    // Méthodes internes privées
    void initMatch();
    void updateHandViews();
    void centerText(sf::Text& text, float x, float y);

public:
    // --- CORRECTION CRITIQUE ICI ---
    // On déclare que le constructeur accepte les équipes (std::vector)
    BattleState(GameController& game, std::vector<Character> p1Team, std::vector<Character> p2Team);

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;
};

#endif