cmake_minimum_required(VERSION 3.16)

project(Pokemon_cpp VERSION 0.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Bibliotheque contenant le moteur de combat, independant de l'interface console.
add_library(pokemon_engine
    src/Types.cpp
    src/Mechanics.cpp
    src/Nature.cpp
    src/Move.cpp
    src/PokemonSpecies.cpp
    src/Pokemon.cpp
    src/Trainer.cpp
    src/DamageCalculator.cpp
    src/Battle.cpp
    src/AI.cpp
    src/GameData.cpp
)

target_include_directories(pokemon_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Warnings volontairement stricts pour detecter les erreurs tot dans le projet.
if(MSVC)
    target_compile_options(pokemon_engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(pokemon_engine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()

# Executable interactif : seule cette cible depend de ConsoleUI.
add_executable(pokemon_cpp
    src/main.cpp
    src/ConsoleUI.cpp
)
target_link_libraries(pokemon_cpp PRIVATE pokemon_engine)

# Tests de non-regression du moteur.
enable_testing()
add_executable(pokemon_tests tests/test_v05.cpp)
target_link_libraries(pokemon_tests PRIVATE pokemon_engine)
add_test(NAME pokemon_v05_tests COMMAND pokemon_tests)

# Documentation HTML optionnelle. L'absence de Doxygen ne bloque jamais le build.
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generation de la documentation Doxygen"
        VERBATIM
    )
endif()
