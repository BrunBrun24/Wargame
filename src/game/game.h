#pragma once

#include "map.h"
#include "player.h"

class Game {
 public:
  Game(int nb_player);

  void start_game();
  void turn_by_turn();
  void play(Player& player);

 private:
  int _nb_player;
  Map _map;
};