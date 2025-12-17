#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Player.h"
#include "CharacterView.h"
#include "CardView.h"
#include "abilities/AllAbilities.h"
#include <vector>
#include <memory>

class BattleState : public State {
public:
    // Enum interne pour le flux du combat
    enum class BattlePhase { InterTurn, WaitingForInput, TurnTransition, CombatEnd };

private:
    // Donn�es du match (d�plac�es depuis GameController)
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

    // M�thodes internes priv�es
    void initMatch();
    void updateHandViews();
    void centerText(sf::Text& text, float x, float y);

public:
    BattleState(GameController& game); // Constructeur initialise le match

    void handleInput(GameController& game, sf::Event& event) override;
    void update(GameController& game) override;
    void render(GameController& game, sf::RenderWindow& window) override;
};

#endif
