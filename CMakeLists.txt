cmake_minimum_required(VERSION 3.14)

project(CodeFighter VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- SPÉCIFIQUE MACOS (Pour toi) ---
if (APPLE)
    # On pointe vers ta version installée par brew (sfml@2 est souvent la 2.6.1, mais ça marchera)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/sfml@2")
endif()

# --- RECHERCHE DES DÉPENDANCES (COMPATIBILITÉ MAXIMALE) ---
# CMake accepte n'importe quelle version >= 2.5 (donc ta 2.6 est valide).
find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

# --- EXÉCUTABLE ---
add_executable(${PROJECT_NAME} src/main.cpp)

# --- INCLUSION ---
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR})

# --- LINKING ---
target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
)

# Frameworks macOS requis
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit"
            "-framework CoreAudio"
            "-framework AudioToolbox"
    )
endif()