cmake_minimum_required(VERSION 3.14)

project(CodeFighter VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- SPÉCIFIQUE MACOS (Apple Silicon & Intel) ---
if (APPLE)
    # On force CMake à chercher la version 2 spécifique installée par Brew
    # (Tes collègues sous Windows n'utiliseront pas ce bloc, donc pas de conflit)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/sfml@2")
endif()

# --- RECHERCHE DES DÉPENDANCES (SFML 2.6) ---
# On demande explicitement la version 2
find_package(SFML 2.6 COMPONENTS graphics audio window system REQUIRED)

# --- EXÉCUTABLE ---
add_executable(${PROJECT_NAME} src/main.cpp)

# --- INCLUSION ---
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR})

# --- LINKING (Syntaxe SFML 2.x) ---
# Note : En version 2, les variables sont ${SFML_LIBRARIES} ou les noms en minuscules
target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
)

# Frameworks macOS requis (seulement sur Mac)
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit"
            "-framework CoreAudio"
            "-framework AudioToolbox"
    )
endif()