#include <SFML/Graphics.hpp>

int main() {
    // 1. Création de la fenêtre (Syntaxe 2.6 standard)
    // Pas d'accolades {} pour le VideoMode en 2.6 !
    sf::RenderWindow window(sf::VideoMode(800, 600), "Code Fighter - Team Build");

    // 2. Boucle principale
    while (window.isOpen()) {
        sf::Event event;

        // Syntaxe Classique : On passe l'événement par référence
        while (window.pollEvent(event)) {
            // Gestion de la fermeture
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // 3. Rendu
        window.clear(sf::Color(50, 50, 50)); // Gris foncé
        // window.draw(...);
        window.display();
    }

    return 0;
}