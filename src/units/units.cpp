#include "units.h"

Unit::Unit(int pv, int speed, int power, int defense,
           std::vector<TerrainsType> allow_terrain)
    : _pv(pv),
      _speed(speed),
      _power(power),
      _defense(defense),
      _range(1),
      _team(Default),
      allow_terrain(allow_terrain) {}

Unit::~Unit() {}

void Unit::attack(Unit* ennemy) {
  if (ennemy->get_team() != _team) {
    ennemy->set_pv(_power);
    this->set_pv(ennemy->get_power() - ennemy->get_def());

    if (ennemy->get_pv() <= 0) {
      delete ennemy;
    }

    if (_pv <= 0) {
      delete this;
    }
  }
};