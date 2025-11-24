cmake_minimum_required(VERSION 3.14)

project(CodeFighter VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- SPÉCIFIQUE MACOS (Apple Silicon & Intel) ---
if (APPLE)
    # Aide CMake à trouver Homebrew
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/sfml")
endif()

# --- RECHERCHE DES DÉPENDANCES (SFML 3) ---
# Note : On cherche la version 3. Si tes collègues sont sous Windows,
# ils devront installer SFML 3.x aussi (pas la 2.5/2.6).
find_package(SFML 3 COMPONENTS Graphics Audio Window System REQUIRED)

# --- DÉFINITION DE L'EXÉCUTABLE ---
add_executable(${PROJECT_NAME} src/main.cpp)

# --- LINKING ---
# Avec SFML 3, on utilise les "Targets" modernes (SFML::...)
target_link_libraries(${PROJECT_NAME} PRIVATE
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
)

# Frameworks macOS requis (seulement sur Mac)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit"
            "-framework CoreAudio"
            "-framework AudioToolbox"
    )
endif()