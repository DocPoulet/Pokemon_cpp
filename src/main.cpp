#include "pokemon/GameApp.hpp"

#include <iostream>

int main() {
    pokemon::GameApp app(std::cin, std::cout);
    return app.run();
}
