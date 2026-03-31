#include "player.h"

#include "algorithm"
#include "units.h"

int Player::_id_counter = 0;

Player::Player(Country country) : _country(country) {
  this->_id = _id_counter++;
}

void Player::add_unit(Unit* unit) {
  if (unit) {
    _units.push_back(unit);
  }
}

void Player::remove_unit(Unit* unit) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit->get_id();
  });

  if (it != _units.end()) {
    _units.erase(it);
  }
}
