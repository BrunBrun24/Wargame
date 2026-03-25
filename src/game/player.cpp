#include "player.h"

int Player::_id_counter = 0;

Player::Player(Country country) : _country(country) {
  this->_id = _id_counter++;
}