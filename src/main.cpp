#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Test de lien avec la SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test CodeFighters V2");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.display();
    }
    return 0; 
}