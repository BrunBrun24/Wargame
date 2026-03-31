#include "player.h"

#include "units.h"
#include "algorithm"

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

void Player::add_building(std::unique_ptr<Terrains> building) {
  if (building) {
    _terrains.push_back(std::move(building));
  }
}
