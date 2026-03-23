#include <iostream>
#include <vector>

#include "terrains.h"

enum Team { Default, Rouge, Bleu };

class Unit {
 public:
  Unit(int pv, int speed, int power, int defense,
       std::vector<TerrainsType> allow_terrain);
  ~Unit();

  void attack(Unit* ennemy);

  Team get_team() { return _team; }
  int get_pv() { return _pv; }
  int get_power() { return _power; }
  void set_pv(int atq) { _pv -= atq; }
  int get_def() { return _defense; }

 protected:
  std::vector<TerrainsType> allow_terrain;

 private:
  Team _team;
  int _pv;
  int _speed;  // Portée de déplacement case par case
  int _power;
  int _defense;
  int _range;  // Portée d'attaque
};