#pragma once

#include <string>

#include "type.h"

class Player;
class Case;
class Unit;
class City;

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

  /** @return La meilleure unité de la case pour affronter l'ennemi */
  Unit* select_best_unit(const Unit* unit) const;

  /** @return Le chemin le plus court pour aller à la case cible */
  Course distance_between(const Case* target_case);

  /**
   * @brief Vérifie si une ville peut être construite ici (distance de 2 cases
   * minimum).
   */
  bool is_valid_for_city() const;
  void create_city(Player* player);

  /** @brief Calcule le rendement total de la case (Terrain + Ressource +
   * Amélioration) */
  Yields get_total_yields() const;
  Yields get_base_yields() const;

  void add_unit(Unit* unit);
  void remove_unit(Unit* unit_to_move);

  /** @brief Ajoute un améanagement sur la case */
  void add_improvement(const ImprovementName improvement) {
    _terrain.improvement = improvement;
  }

  void add_neighbor(Case* neighbor);

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
};