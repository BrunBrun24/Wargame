#pragma once

#include "map.h"
#include "player.h"

class Game {
 public:
  Game(int nb_player);

 private:
  int _nb_player;
  std::vector<std::unique_ptr<Player>> _players;
};