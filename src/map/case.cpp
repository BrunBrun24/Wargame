#include "case.h"

void Case::add_neighbor(Case* neighbor) {
  if (neighbor != nullptr) {
    _neighbors.push_back(neighbor);
  }
}

void Case::add_unit(Case& c, Unit& unit) { c._units.push_back(&unit); }

bool Case::movement_is_possible(const Case& target_case,
                                const Unit unit) const {
  if (this == &target_case) return true;

  std::vector<const Case*> visited;
  return movement_is_possible_rec(target_case, unit, unit.get_speed(), visited);
}

bool Case::movement_is_possible_rec(const Case& target_case, const Unit unit,
                                    int speed,
                                    std::vector<const Case*>& visited) const {
  if (speed <= 0) {
    return false;
  }

  visited.push_back(this);

  for (const Case* neighbor : _neighbors) {
    // Vérification si le terrain du voisin est praticable
    if (!unit.find_terrain(neighbor->_terrains.get_terrain_type())) {
      continue;
    }

    if (std::find(visited.begin(), visited.end(), neighbor) != visited.end()) {
      continue;
    }

    if (neighbor == &target_case) {
      return true;
    }

    if (neighbor->movement_is_possible_rec(target_case, unit, speed - 1,
                                           visited)) {
      return true;
    }
  }

  return false;
}

void Case::movement(Case& target_case, Unit& unit_to_move) {
  if (!movement_is_possible(target_case, unit_to_move)) return;

  delete_unit(target_case, unit_to_move);
  add_unit(target_case, unit_to_move);
}

void Case::delete_unit(Case& target_case, Unit& unit_to_move) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit_to_move.get_id();
  });

  if (it != _units.end()) {
    target_case._units.push_back(*it);
    _units.erase(it);
  }
}
