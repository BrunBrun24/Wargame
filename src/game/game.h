#pragma once

#include <QMap>

#include "map.h"
#include "player.h"

class Unit;

class Game {
 public:
  Game(int nb_player);
  // Un Game qui prend une liste de pays pour les joueurs. Game(Country*
  // Joueurs);
  Game(const QMap<int, Country>& PaysDesJoueurs);
  void start_game();
  void turn_by_turn();
  void play(Player* player);
  Map* getMap() { return &_map; }

  void update_input(Player* player);
  void next_unit(Player* player);
  
  signals:
    bool user_clicked_on_screen();
    Unit* get_unit_at_click();

 private:
  int _nb_player;
  Map _map;
};