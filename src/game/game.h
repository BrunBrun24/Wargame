#pragma once

#include <QMap>
#include <vector>

#include "map.h"
#include "player.h"

class Unit;

class Game {
 public:
  Game(int nb_player);
  Game(const QMap<int, Country>& PaysDesJoueurs);

  /** @brief Initialise la carte et récupère les joueurs. */
  void start_game();

  /** @brief Passe au joueur suivant et initialise son tour. */
  void next_turn();

  /** @return Le pointeur vers le joueur actif. */
  Player* get_current_player();
  std::vector<Player*> get_players() const { return _players_list; }

  Map* getMap() { return &_map; }

 private:
  int _nb_player;
  Map _map;
  int _current_player_index = 0;
  int _turn_count = 1;
  std::vector<Player*> _players_list;
};