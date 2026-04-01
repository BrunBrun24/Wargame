#pragma once

#include <string>
#include <vector>

#include "type.h"

class Player;
class Case;
class Unit;

struct Course {
  bool is_possible;
  std::vector<Case*> distance_traveled;
};

class Case {
 public:
  Case(TerrainsType type = TerrainsType::Ocean, Player* player = nullptr,
       Country country = Country::Neutral);
  ~Case() = default;

  void add_neighbor(Case* neighbor);
  void add_unit(Unit* unit);
  void remove_unit(Unit* unit_to_move);

  bool create_city_is_possible(Case* target_case, Unit* unit);
  /** @brief Transforme un colon en ville et détruit l'unité. */
  void create_city(Case* target_case, Unit* settler);

  bool create_building_is_possible(Case* target_case, Unit* unit);
  void create_building(Case* target_case, Unit* worker, BuildingName building);

  char get_debug_char() const;

  /** @brief Détermine si l'on peut atteindre la case en partant de l'unité */
  Course movement_is_possible(Case* target_case, const Unit* unit);

  /** @brief Déplace l'unité sur la case */
  void movement(Case* target_case, Unit* unit_to_move);

  /** @return La meilleure unité de la case pour affronter l'ennemi */
  Unit* select_best_unit(Unit* ennemy) const;

  /** @return Le chemin le plus court pour aller à la case cible */
  Course distance_between(Case* target_case);

  /** @return Le chemin le plus court pour trouver le type d'un bâtiment */
  Course calculate_first_building_distance(BuildingName building);

  void set_player(Player* player) { _player = player; }
  void set_country(Country country) { _country = country; }
  void set_terrain(TerrainsType type) { _terrain = type; }
  void set_building(BuildingName building) { _building = building; }
  void set_resource(ResourceName res) { _resource = res; }

  int get_id() const { return _id; }
  Player* get_player() { return _player; };
  Country get_country() { return _country; };
  TerrainsType get_terrain() const { return _terrain; }
  BuildingName get_building() const { return _building; }
  ResourceName get_resource() const { return _resource; }
  const std::vector<Case*> get_neighbors() const { return _neighbors; }
  std::vector<Unit*> get_units() const { return _units; }
  Country get_unit_country() const;
  std::string get_description() const;

 private:
  void _capture_and_displace(Case* target_case, Unit* unit_to_move);

  static int _id_counter;

  int _id;
  Player* _player;   // Joueur possédant cette case
  Country _country;  // Pays des unités sur la case
  TerrainsType _terrain;
  BuildingName _building;
  ResourceName _resource;
  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unités sur la case
};