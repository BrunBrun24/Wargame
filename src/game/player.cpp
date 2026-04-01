#include "player.h"

#include "algorithm"
#include "case.h"
#include "units.h"

int Player::_id_counter = 0;

Player::Player(Country country) : _country(country) {
  this->_id = _id_counter++;
}

Player::~Player() {
  // 1. On utilise ta fonction clear_units qui fait déjà les 'delete'
  clear_units();

  // 2. On rend neutres les cases où il y avait des bâtiments
  for (Case* c : _buildings) {
    if (c != nullptr) {
      c->set_player(nullptr);
    }
  }
  _buildings.clear();
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

void Player::clear_units() {
  for (Unit* unit : _units) {
    if (unit != nullptr) {
      // 1. On prévient la case que l'unité n'existe plus
      if (unit->get_case_unit() != nullptr) {
        unit->get_case_unit()->remove_unit(unit);
      }
      // 2. On libère la mémoire
      delete unit;
    }
  }
  // 3. On vide le vecteur d'adresses désormais invalides
  _units.clear();
}

void Player::add_building(Case* c) {
  if (c) {
    _buildings.push_back(c);
  }
}

void Player::remove_building(Case* c) {
  // On cherche le bâtiment par son ID
  auto it = std::find_if(_buildings.begin(), _buildings.end(),
                         [&](Case* ca) { return ca->get_id() == c->get_id(); });

  if (it != _buildings.end()) {
    _buildings.erase(it);
  }
}
