cmake_minimum_required(VERSION 3.16)

project(Pokemon_cpp VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(pokemon_engine
    Workshop_POO_pt3.cpp
    EffetsAttaques.cpp
    Liste_Attaques.cpp
    Pokedex_1g.cpp
    Terrain.cpp
)

target_include_directories(pokemon_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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

add_executable(pokemon_cpp main.cpp)
target_link_libraries(pokemon_cpp PRIVATE pokemon_engine)

enable_testing()

add_executable(pokemon_tests tests/test_v02.cpp)
target_link_libraries(pokemon_tests PRIVATE pokemon_engine)
add_test(NAME pokemon_v02_tests COMMAND pokemon_tests)
