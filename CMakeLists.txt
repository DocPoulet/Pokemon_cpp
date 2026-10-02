cmake_minimum_required(VERSION 3.16)

project(Pokemon_cpp VERSION 0.8.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Bibliotheque contenant le moteur de combat, independant de l'interface console.
add_library(pokemon_engine
    src/JsonLite.cpp
    src/DataCodec.cpp
    src/Localization.cpp
    src/ShowdownImporter.cpp
    src/ShowdownExporter.cpp
    src/BattlePokemonData.cpp
    src/Types.cpp
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

add_executable(pokemon_tests_v072 tests/test_v072.cpp)
target_link_libraries(pokemon_tests_v072 PRIVATE pokemon_engine)
add_test(NAME pokemon_v072_tests COMMAND pokemon_tests_v072)

add_executable(pokemon_tests_v073 tests/test_v073.cpp)
target_link_libraries(pokemon_tests_v073 PRIVATE pokemon_engine)
add_test(NAME pokemon_v073_tests COMMAND pokemon_tests_v073)

add_executable(pokemon_tests_v074 tests/test_v074.cpp)
target_link_libraries(pokemon_tests_v074 PRIVATE pokemon_engine)
add_test(NAME pokemon_v074_tests COMMAND pokemon_tests_v074)

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

add_executable(pokemon_tests_v075 tests/test_v075.cpp)
target_link_libraries(pokemon_tests_v075 PRIVATE pokemon_engine)
add_test(NAME pokemon_v075_tests COMMAND pokemon_tests_v075)

add_executable(pokemon_tests_v08 tests/test_v08.cpp)
target_link_libraries(pokemon_tests_v08 PRIVATE pokemon_engine)
add_test(NAME pokemon_v08_tests COMMAND pokemon_tests_v08)

# Synchronisation optionnelle des catalogues depuis Pokemon Showdown.
find_package(Python3 COMPONENTS Interpreter QUIET)
if(Python3_Interpreter_FOUND)
    add_custom_target(sync_catalogs
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/sync_showdown_catalogs.py --output ${CMAKE_CURRENT_SOURCE_DIR}/data
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Synchronisation des catalogues Pokemon Showdown"
        VERBATIM
    )
endif()

add_executable(pokemon_tests_v081 tests/test_v081.cpp)
target_link_libraries(pokemon_tests_v081 PRIVATE pokemon_engine)
add_test(NAME pokemon_v081_tests COMMAND pokemon_tests_v081)
