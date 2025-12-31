#include <SFML/Graphics.hpp>
#include "../include/controllers/GameController.h"
#include "core/ResourceManager.h"

int main() {
    // Game initialization and main loop
    GameController game;
    game.run();

    return 0;
}
