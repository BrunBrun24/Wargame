#include "game.h"

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

  while (players.size() > 1) {
    for (const auto& player : players) {
    }
  }
}

void Game::play(Player& player) {
  // On récupère toutes les unités du joueur

  // Tant que toutes les unités ne sont pas inactif
}