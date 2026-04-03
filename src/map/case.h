#pragma once

#include <string>
#include <vector>

#include "type.h"

class Player;
class Case;
class Unit;
class City;

struct Course {
  bool is_possible;
  std::vector<Case*> distance_traveled;
};

class Case {
 public:
  Case(TerrainsType type = TerrainsType::Ocean, Player* player = nullptr,
       Country country = Country::Neutral);
  ~Case() = default;

  /** @brief Détermine si l'on peut atteindre la case en partant de l'unité */
  Course movement_is_possible(const Case* target_case, const Unit* unit);

  /** @brief Déplace l'unité sur la case */
  void movement(Case* target_case, Unit* unit_to_move);

  /** @return La meilleure unité de la case pour affronter l'ennemi */
  Unit* select_best_unit(const Unit* ennemy) const;

  /** @return Le chemin le plus court pour aller de la case actuelle à la case
   * cible */
  Course distance_between(const Case* target_case);

  /** @return Le chemin le plus court pour trouver le bâtiment le plus proche */
  Course calculate_first_building_distance(const BuildingName building);

  /** @return Le chemin le plus court pour trouver la ville la plus proche */
  Course calculate_first_city_distance();

  /** @brief Calcule le rendement total de la case (Terrain + Ressource +
   * Amélioration) */
  Yields get_total_yields() const;

  void add_unit(Unit* unit);
  void remove_unit(Unit* unit_to_move);

  void add_neighbor(Case* neighbor);

  bool create_city_is_possible(const Case* target_case, const Unit* unit);
  void create_city(Case* target_case, Unit* settler);

  bool create_building_is_possible(const Case* target_case, const Unit* unit);
  void create_building(Case* target_case, Unit* worker,
                       const BuildingName building);

  Yields get_base_yields() const;
  Country get_unit_country() const;
  std::string get_description() const;

  char get_debug_char() const;
  int get_id() const { return _id; }

  Country get_country() const { return _country; };
  void set_country(Country country) { _country = country; }

  TerrainsType get_terrain() const { return _terrain; }
  void set_terrain(TerrainsType type) { _terrain = type; }

  Player* get_player() const { return _player; };
  void set_player(Player* player) { _player = player; }

  BuildingName get_building() const { return _building; }
  void set_building(BuildingName building) { _building = building; }

  ResourceName get_resource() const { return _resource; }
  void set_resource(ResourceName res) { _resource = res; }

  City* get_city() const { return _city; }
  void set_city(City* city) { _city = city; }

  std::vector<Case*> get_neighbors() const { return _neighbors; }
  std::vector<Unit*> get_units() const { return _units; }

 private:
  static int _id_counter;

  int _id;
  Country _country;  // Pays des unités sur la case
  TerrainsType _terrain;
  ResourceName _resource;
  BuildingName _building;
  Player* _player;  // Joueur possédant cette case
  City* _city;
  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unités sur la case

  /** @brief On capture les unités civiles et on déplace l'unité qui les a
   * capturées */
  void _capture_and_displace(Case* target_case, Unit* unit_to_move);
};