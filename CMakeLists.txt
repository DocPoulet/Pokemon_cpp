cmake_minimum_required(VERSION 3.16)

project(Pokemon_cpp VERSION 0.7.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Bibliotheque contenant le moteur de combat, independant de l'interface console.
add_library(pokemon_engine
    src/JsonLite.cpp
    src/DataCodec.cpp
    src/BattlePokemonData.cpp
    src/Types.cpp
    src/TeamIO.cpp
    src/TeamBuilder.cpp
    src/GameMode.cpp
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
target_compile_definitions(pokemon_engine PRIVATE POKEMON_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

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
    src/GameApp.cpp
)
target_link_libraries(pokemon_cpp PRIVATE pokemon_engine)

# Tests de non-regression du moteur.
enable_testing()
add_executable(pokemon_tests_v05 tests/test_v05.cpp)
target_link_libraries(pokemon_tests_v05 PRIVATE pokemon_engine)
add_test(NAME pokemon_v05_tests COMMAND pokemon_tests_v05)

add_executable(pokemon_tests_v06 tests/test_v06.cpp)
target_link_libraries(pokemon_tests_v06 PRIVATE pokemon_engine)
add_test(NAME pokemon_v06_tests COMMAND pokemon_tests_v06)

add_executable(pokemon_tests_v07 tests/test_v07.cpp)
target_link_libraries(pokemon_tests_v07 PRIVATE pokemon_engine)
add_test(NAME pokemon_v07_tests COMMAND pokemon_tests_v07)

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
