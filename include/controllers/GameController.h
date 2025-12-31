#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "../states/State.h"
#include <SFML/Audio.hpp>

class GameController {
public:
    sf::RenderWindow window;
    // sf::Font font; // <-- Removed
    sf::Music MenuMusic;
    sf::Music BattleMusic;
    float globalVolume = 50.0f;

    // THE CORE OF THE STATE PATTERN
    std::unique_ptr<State> currentState;

    GameController();
    ~GameController() = default;

    void run();
    void changeState(std::unique_ptr<State> newState); // To switch from Menu to Battle

    // Shared resources
    void initResources();

    void setMusicVolume(float volume);
    void playMenuMusic();
    void playBattleMusic();
    void stopMusics();
};

#endif