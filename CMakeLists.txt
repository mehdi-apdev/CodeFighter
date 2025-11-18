# --- CONFIGURATION GLOBALE DU PROJET ---
# Pas besoin de politiques étendues, on fait tout à la main.
cmake_minimum_required(VERSION 3.14)

project(CodeFighter VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- DÉFINITION MANUELLE DES CHEMINS (À ADAPTER SI VOUS BOUGEZ LE DOSSIER) ---
# Le chemin que vous avez créé
set(SFML_ROOT_DIR "/Users/melmestari/Developer/SDKs/SFML-2.5.1-macos-clang")

# Répertoires des en-têtes et des bibliothèques
set(SFML_INCLUDE_DIR ${SFML_ROOT_DIR}/include)
set(SFML_LIBRARY_DIR ${SFML_ROOT_DIR}/lib)

# --- EXÉCUTABLE & LINKING MANUEL ---

# Liste des fichiers source
add_executable(
        ${PROJECT_NAME}
        main.cpp
)

# 1. Dire au compilateur où trouver les en-têtes (#include <SFML/...)
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR})

# 2. Dire à l'éditeur de liens où trouver les fichiers .dylib
target_link_directories(${PROJECT_NAME} PUBLIC ${SFML_LIBRARY_DIR})

# 3. Lier les bibliothèques SFML + les dépendances système macOS (OBLIGATOIRE !)
target_link_libraries(
        ${PROJECT_NAME}
        # Bibliothèques SFML 2.5.1
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio

        # Frameworks système requis par SFML sur macOS
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreAudio"
        "-framework AudioToolbox"
)