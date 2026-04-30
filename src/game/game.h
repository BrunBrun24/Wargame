#pragma once

#include "map.h"
#include "player.h"
#include <QMap>
class Game {
 public:
  Game(int nb_player);
  //Un Game qui prend une liste de pays pour les joueurs. Game(Country* Joueurs);
  Game(const QMap<int,Country>& PaysDesJoueurs);
  void start_game();
  void turn_by_turn();
  void play(Player& player);
  Map* getMap() { return &_map; }

 private:
  int _nb_player;
  Map _map;
};