#include "units.h"

#include <algorithm>

int Unit::_id_counter = 0;

Unit::Unit(UnitName name, Country country, int pv, int speed, int power,
           int defense, int range, std::vector<TerrainsType> allow_terrain)
    : _name(name),
      _pv(pv),
      _country(country),
      _speed(speed),
      _power(power),
      _defense(defense),
      _range(range),
      allow_terrain(allow_terrain) {
  this->_id = _id_counter++;
}

Unit::~Unit() {}

void Unit::attack(Unit* ennemy) {
  if (ennemy->get_country() != _country) {
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

bool Unit::find_terrain(const TerrainsType& target_terrain) const {
  auto it =
      std::find(allow_terrain.begin(), allow_terrain.end(), target_terrain);

  return it != allow_terrain.end();
}