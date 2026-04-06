#pragma once

#include <string>

#include "type.h"

class Player;
class Case;
class Unit;
class City;

struct Course {
  bool is_possible;
  std::vector<Case*> distance_traveled;
};

struct Terrain {
  TerrainsType type = TerrainsType::Ocean;
  TerrainElevation elevation = TerrainElevation::Flat;
  TerrainFeature feature = TerrainFeature::None;
  ImprovementName improvement = ImprovementName::None;
  ResourceName resource = ResourceName::None;
  double defense_bonus = 0;

  void set_defense(double defense) {
    this->defense_bonus = std::max(0.0, defense_bonus + defense);
  }

  /**
   * @brief Calcule le coût en points de mouvement pour traverser ce terrain.
   * @return 2 si présence d'une feature ou d'une colline, 1 sinon.
   */
  int calculate_move_cost() const {
    if (feature != TerrainFeature::None ||
        elevation == TerrainElevation::Hill) {
      return 2;
    }
    return 1;
  }
};

class Case {
 public:
  Case(Terrain terrain = {}, Country country = Country::Neutral,
       Player* player = nullptr);
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
  Course calculate_first_improvement_distance(
      const ImprovementName improvement);

  /** @return Le chemin le plus court pour trouver la ville la plus proche */
  Course calculate_first_city_distance();

  /** @brief Calcule le rendement total de la case (Terrain + Ressource +
   * Amélioration) */
  Yields get_total_yields() const;
  Yields get_base_yields() const;

  void add_unit(Unit* unit);
  void remove_unit(Unit* unit_to_move);

  void add_neighbor(Case* neighbor);

  bool create_city_is_possible(const Case* target_case, const Unit* unit);
  void create_city(Case* target_case, Unit* settler);

  Country get_unit_country() const;
  std::string get_description() const;

  int get_id() const { return _id; }
  Terrain& get_terrain() { return _terrain; };
  const Terrain& get_terrain() const { return _terrain; }

  Country get_country() const { return _country; };
  void set_country(Country country) { _country = country; }

  Player* get_player() const { return _player; };
  void set_player(Player* player) { _player = player; }

  City* get_city() const { return _city; }
  void set_city(City* city) { _city = city; }

  std::vector<Case*> get_neighbors() const { return _neighbors; }
  std::vector<Unit*> get_units() const { return _units; }

 private:
  static int _id_counter;

  int _id;
  Terrain _terrain;               // Spécificité du terrain
  Country _country;               // Pays des unités sur la case
  Player* _player;                // Joueur possédant cette case
  City* _city;                    // Ville sur la case
  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unités sur la case

  /** @brief On capture les unités civiles et on déplace l'unité qui les a
   * capturées */
  void _capture_and_displace(Case* target_case, Unit* unit_to_move);
};