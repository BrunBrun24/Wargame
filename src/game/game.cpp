#include "game.h"

#include "player_utils.h"

Game::Game(int nb_player) : _nb_player(nb_player) {
  std::vector<Country> taken_countries;

  // On créer le nombre de joueur demandé
  for (int i = 0; i < nb_player; i++) {
    std::cout << "\nJoueur " << i + 1;

    // On passe la liste des pays déjà pris
    Country c = PlayerParser::choice_country(taken_countries);

    // On ajoute le nouveau pays à la liste d'exclusion pour le prochain tour
    taken_countries.push_back(c);

    _players.push_back(std::make_unique<Player>(c));
  }
}