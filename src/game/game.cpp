#include "game.h"

#include <iostream>

Game::Game(int nb_player) : _nb_player(nb_player), _map(nb_player) {}

void Game::start_game() {
  // On créer la map
  _map.create_map();
  _map.render_debug();

  // std::cout << "Le joueur n°" << players.at(0)->get_id() << " à gagné" <<
  // std::endl;
}

void Game::turn_by_turn() {
  std::vector<Player*> players = _map.get_players();
  int turn_count = 1;

  // Boucle principale du jeu
  while (players.size() > 1) {
    std::cout << "\n========== TOUR " << turn_count
              << " ==========" << std::endl;

    for (Player* player : players) {
      if (player == nullptr) continue;

      std::cout << "\nC'est au tour de : " << player->get_id() << std::endl;

      player->initialise_turn();

      // 2. Phase d'interaction
      this->play(*player);
    }
    turn_count++;
  }
}

void Game::play(Player& player) {
  // On récupère toutes les unités du joueur

  // Tant que toutes les unités ne sont pas inactif
}