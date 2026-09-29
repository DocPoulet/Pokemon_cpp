#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"
#include "pokemon/ConsoleUI.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/Nature.hpp"
#include "pokemon/Pokemon.hpp"
#include "pokemon/Trainer.hpp"

#include <iostream>

using namespace pokemon;

int main() {
    GameData data;

    Trainer docPoulet("DocPoulet");
    Trainer frodon("Frodon");

    Pokemon salameche1(&data.species("Salameche"), 5, Nature("Hardi"));
    Pokemon carapuce1(&data.species("Carapuce"), 5,
                      Nature("Modeste", Stat::SpecialAttack, Stat::Attack));
    Pokemon salameche2(&data.species("Salameche"), 5,
                       Nature("Timide", Stat::Speed, Stat::Attack));
    Pokemon carapuce2(&data.species("Carapuce"), 5,
                      Nature("Assure", Stat::Defense, Stat::Attack));

    salameche1.setMove(0, &data.move("Zenith"));
    salameche1.setMove(1, &data.move("Flammeche"));
    salameche1.setMove(2, &data.move("Aiguisage"));
    salameche1.setMove(3, &data.move("Close Combat"));

    carapuce2.setMove(0, &data.move("Fouet Lianes"));
    carapuce2.setMove(1, &data.move("Pistolet a O"));
    carapuce2.setMove(2, &data.move("Champ Herbu"));

    salameche2.setMove(0, &data.move("Flammeche"));
    salameche2.setMove(1, &data.move("Griffe"));
    salameche2.setMove(2, &data.move("Fouet Lianes"));

    carapuce1.setMove(0, &data.move("Fouet Lianes"));
    carapuce1.setMove(1, &data.move("Pistolet a O"));
    carapuce1.setMove(2, &data.move("Psyko"));

    docPoulet.addPokemon(salameche1);
    docPoulet.addPokemon(carapuce2);
    frodon.addPokemon(salameche2);
    frodon.addPokemon(carapuce1);

    Battle battle(docPoulet, frodon);
    GreedyAI ai;
    ConsoleUI ui(std::cin, std::cout);
    ui.run(battle, ai);
    return 0;
}
