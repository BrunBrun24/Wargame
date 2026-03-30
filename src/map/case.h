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
  Case() = default;
  Case(TerrainsType type);
  Case(TerrainsType type, Country country);

  void add_neighbor(Case* neighbor);
  void add_unit(Unit* unit);
  void delete_unit(Unit* unit_to_move);

  bool create_city_is_possible(Case* target_case, Unit* unit);
  void create_city(Case* target_case, Unit* unit);

  /** @brief Détermine si l'on peut atteindre la case en partant de l'unité */
  Course movement_is_possible(Case* target_case, const Unit* unit);

  /** @brief Déplace l'unité sur la case */
  void movement(Case* target_case, Unit* unit_to_move);

  /** @return La meilleure unité de la case pour affronter l'ennemi */
  Unit* select_best_unit(Unit* ennemy) const;

  /** @return Le chemin le plus court pour aller à la case ciblé */
  Course distance_between(const Case& target) const;

  /** @return Le chemin le plus court pour trouver le type d'un bâtiment */
  Course _calculate_first_building_distance(BuildingType type);

  void set_country(Country country) { _country = country; }
  void set_terrain_type(TerrainsType type) { _terrains.set_terrain(type); }
  void set_country_neutral();

  Country get_country() { return _country; };
  Terrains& get_terrain() { return _terrains; };
  const std::vector<Case*> get_neighbors() const { return _neighbors; }
  Country get_unit_country() const;
  std::string get_description();

 private:
  Course _distance_between_rec(const Case& target,
                               std::vector<const Case*>& visited) const;

  Country _country;
  Terrains _terrains;
  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unités sur la case
};