#pragma once

#include <vector>

#include "terrains.h"
#include "units.h"

class Case;

struct Course {
  bool is_possible;
  std::vector<Case*> distance_traveled;
};

class Case {
 public:
  Case();
  Case(TerrainsType type);

  void add_neighbor(Case* neighbor);
  void add_unit(Unit& unit) { _units.push_back(&unit); };
  void delete_unit(Unit& unit_to_move);

  const std::vector<Case*>& get_neighbors() const { return _neighbors; }
  Country get_unit_country() const;
  Terrains get_terrain() { return _terrains; };

  void Case::set_terrain_type(TerrainsType type) {
    _terrains.set_terrain(type);
  }

  Course movement_is_possible(const Case& target_case, const Unit& unit) const;
  Course movement_is_possible_rec(const Case& target_case, const Unit& unit,
                                  int speed,
                                  std::vector<const Case*>& visited) const;
  void movement(Case& target_case, Unit& unit_to_move);

  Unit& select_best_unit(Unit& ennemy) const;
  Course distance_between(const Case& target) const;

 private:
  Course _distance_between_rec(const Case& target,
                               std::vector<const Case*>& visited) const;

  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unité sur la case
  Terrains _terrains;
};