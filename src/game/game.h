#pragma once

#include "map.h"
#include "player.h"

class Game {
 public:
  Game(int nb_player);

  void start_game();

 private:
  int _nb_player;
  std::vector<std::unique_ptr<Player>> _players;
  Map _map;
};