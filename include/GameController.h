#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "State.h"
#include <SFML/Audio.hpp>

class GameController {
public:
    sf::RenderWindow window;
    sf::Font font;
    sf::Music MenuMusic;
    sf::Music BattleMusic;
    float globalVolume = 50.0f;

    // LE COEUR DU PATTERN STATE
    std::unique_ptr<State> currentState;

    GameController();
    ~GameController() = default;

    void run();
    void changeState(std::unique_ptr<State> newState); // Pour passer de Menu à Battle

    // Ressources partagées
    void initResources();

    void setMusicVolume(float volume);
    void playMenuMusic();
    void playBattleMusic();
    void stopMusics();
};

#endif
