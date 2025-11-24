#include <SFML/Graphics.hpp>
#include <optional> // Requis par SFML 3 : pollEvent retourne maintenant un std::optional

int main() {
    // --- 1. INITIALISATION DE LA FENÊTRE ---
    // En SFML 3, la taille (Vector2u) doit être passée entre accolades {}.
    // Cela crée implicitement un objet sf::Vector2u(800, 600).
    auto mode = sf::VideoMode({800, 600});

    sf::RenderWindow window(mode, "Code Fighter - Pre-Alpha");

    // --- 2. BOUCLE DE JEU (GAME LOOP) ---
    // Cette boucle tourne tant que la fenêtre est ouverte.
    while (window.isOpen()) {

        // --- 3. GESTION DES INPUTS (POLLING) ---
        // window.pollEvent() ne bloque pas le programme.
        // Elle retourne un std::optional contenant l'événement s'il y en a un,
        // ou "std::nullopt" (vide) s'il n'y en a plus.
        while (const std::optional event = window.pollEvent()) {

            // SFML 3 remplace l'enum "event.type" par une méthode template ".is<T>()".
            // C'est plus robuste ("type-safe"). Ici, on vérifie si l'utilisateur a cliqué sur la croix.
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- 4. RENDU GRAPHIQUE ---
        // A. Nettoyage : On efface l'image précédente avec une couleur de fond (Gris foncé).
        window.clear(sf::Color(50, 50, 50));

        // B. Dessin : C'est ici qu'on affichera les personnages et cartes plus tard.
        // window.draw(playerSprite);

        // C. Affichage : On bascule le buffer caché vers l'écran visible (Double Buffering).
        window.display();
    }

    return 0;
}