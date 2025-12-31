#include "core/ResourceManager.h" // Corrected include path

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

sf::Texture& ResourceManager::getTexture(const std::string& path) {
    if (m_textures.find(path) == m_textures.end()) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            m_textures[path] = texture;
        } else {
            std::cerr << "ResourceManager Error: Failed to load texture from " << path << std::endl;
            m_textures[path] = sf::Texture(); 
        }
    }
    return m_textures.at(path);
}

sf::Font& ResourceManager::getFont(const std::string& path) {
    if (m_fonts.find(path) == m_fonts.end()) {
        sf::Font font;
        if (font.loadFromFile(path)) {
            m_fonts[path] = font;
        } else {
            std::cerr << "ResourceManager Error: Failed to load font from " << path << std::endl;
            m_fonts[path] = sf::Font();
        }
    }
    return m_fonts.at(path);
}
