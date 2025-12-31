#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <iostream>

class ResourceManager {
public:
    // Gets the singleton instance.
    static ResourceManager& getInstance();

    // Deleted copy constructor and assignment operator to prevent copying
    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;

    // Load a texture from a file and store it with a given name.
    void loadTexture(const std::string& name, const std::string& filename);

    // Get a stored texture by name.
    sf::Texture& getTexture(const std::string& name);

    // Load a font from a file and store it with a given name.
    void loadFont(const std::string& name, const std::string& filename);

    // Get a stored font by name.
    sf::Font& getFont(const std::string& name);

private:
    // Private constructor for the singleton pattern.
    ResourceManager() = default;

    // Maps to store the resources.
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
};

#endif // RESOURCEMANAGER_H
