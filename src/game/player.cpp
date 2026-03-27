#include "player.h"

#include "units.h"

int Player::_id_counter = 0;

Player::Player(Country country) : _country(country) {
  this->_id = _id_counter++;
}

void Player::add_unit(std::unique_ptr<Unit> unit) {
  if (unit) {
    _units.push_back(std::move(unit));
  }
}

void Player::add_building(std::unique_ptr<Terrains> building) {
  if (building) {
    _terrains.push_back(std::move(building));
  }
}
